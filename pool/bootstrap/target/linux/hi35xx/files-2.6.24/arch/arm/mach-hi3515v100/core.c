/* linux/arch/arm/mach-hi3515v100/core.c
*
* Copyright (c) 2006 Hisilicon Co., Ltd. 
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*
*/
#include <linux/autoconf.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/sysdev.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/amba/bus.h>
#include <linux/amba/clcd.h>
#include <linux/clocksource.h>
#include <linux/clockchips.h>

#include <asm/hardware/arm_timer.h>
#include <asm/mach/irq.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>
#include <asm/mach/flash.h>
#include <asm/mach/map.h>
#include <asm/arch/platform.h>
#include <asm/arch/irqs.h>
#include <asm/arch/io.h>
#include <asm/arch/timer.h>
#include <asm/arch/led.h>
#include <asm/arch/early-debug.h>
#include <asm/proc-fns.h>
#include <asm/cpu-single.h>
#include <asm/irq.h>
#include <asm/leds.h>
#include <asm/system.h>
#include <asm/hardware.h>
#include <asm/io.h>

#define ioaddr_intc(x) IO_ADDRESS(REG_BASE_INTC + (x))
unsigned long hisilicon_irq_prio_masks[8] = {
	(1<<INTNR_VOU) | (1<<INTNR_VIU),
	(1<<INTNR_DMAC) | (1<<INTNR_VEDU),
	(1<<INTNR_TDE),
	(1<<INTNR_ETH) | (1<<INTNR_SATA) | (1<<INTNR_SDIO),
	~0,
	0,
};
#ifdef CONFIG_HI_PRIO_IRQ
extern int hi_vic_status;
extern unsigned int hi_vic_mask[NR_IRQS];
extern unsigned int hi_prio_stack[NR_IRQS];
extern struct hi_irq_prio hi_irq_prio_array[NR_IRQS];
#endif 

#define do_level_IRQ    handle_level_irq
static void hil_ack_irq(unsigned int irq)
{
        irq -= INTNR_IRQ_START;
        writel(1<<irq, ioaddr_intc(REG_INTC_INTENCLEAR));
}

static void hil_mask_irq(unsigned int irq)
{
	irq -= INTNR_IRQ_START;
#ifdef CONFIG_HI_PRIO_IRQ
	if(hi_vic_status >= 0){
		int mask = ~(1 << irq);
		int i;
		for(i = hi_vic_status; i >= 0; i--){
			hi_vic_mask[i] &= mask;
		}
	}
#endif
	writel(1<<irq, ioaddr_intc(REG_INTC_INTENCLEAR));
}

static void hil_unmask_irq(unsigned int irq)
{
	irq -= INTNR_IRQ_START;
#ifdef CONFIG_HI_PRIO_IRQ
	if(hi_vic_status >= 0 ){
		/* Get current irq's priority from hi_irq_prio_array */
		int prio = hi_irq_prio_array[irq].prio;
		int i;

		/* from top because high prio at top */
		for (i = hi_vic_status; i >=0; i--){
			/* find if this irq's priority unmasked by vic disable*/
			if(hi_prio_stack[i] < prio){
				hi_vic_mask[i] |= (1 << irq);
				return;
			}
		}
	}
#endif 
	writel(1<<irq, ioaddr_intc(REG_INTC_INTENABLE));
}

static struct irq_chip hic_chip = {
	.ack	= hil_ack_irq,
	.mask	= hil_mask_irq,
	.unmask	= hil_unmask_irq,
	.disable = hil_mask_irq,
};


void __init hisilicon_init_irq(void)
{
	unsigned int i;

	edb_trace(1);

	writel(~0, ioaddr_intc(REG_INTC_INTENCLEAR));
	writel(0, ioaddr_intc(REG_INTC_INTSELECT));
	writel(~0, ioaddr_intc(REG_INTC_SOFTINTCLEAR));
	writel(1, ioaddr_intc(REG_INTC_PROTECTION));

	for (i = INTNR_IRQ_START; i <= INTNR_IRQ_END; i++)
	{
		set_irq_chip(i, &hic_chip );
		set_irq_handler(i, do_level_IRQ);
		set_irq_flags(i, IRQF_VALID | IRQF_PROBE);
	}
}

void arch_idle(void)
{
	cpu_do_idle();
}

void arch_reset(char mode)
{
	writel(~0x0,hisilicon_sctl_reg(REG_SC_SYSSTAT));
}
#define HIL_AMBADEV_NAME(name) hil_ambadevice_##name

#define HIL_AMBA_DEVICE(name,busid,base,platdata)		\
static struct amba_device HIL_AMBADEV_NAME(name) = {		\
	.dev		= {					\
		.coherent_dma_mask = ~0,			\
		.bus_id	= busid,				\
		.platform_data = platdata,			\
	},							\
	.res		= {					\
		.start	= REG_BASE_##base,			\
		.end	= REG_BASE_##base + REG_##base##_IOSIZE -1,	\
		.flags	= IORESOURCE_IO,			\
	},							\
	.dma_mask	= ~0,					\
	.irq		= { INTNR_##base, NO_IRQ }		\
}

#define UART0_IRQ	{ INTNR_UART0, NO_IRQ }
#define UART1_IRQ	{ INTNR_UART1, NO_IRQ }
#define UART2_IRQ	{ INTNR_UART2, NO_IRQ }

HIL_AMBA_DEVICE(uart0, "uart:0",  UART0,    NULL);
HIL_AMBA_DEVICE(uart1, "uart:1",  UART1,    NULL);
HIL_AMBA_DEVICE(uart2, "uart:2",  UART2,    NULL);

static struct amba_device *amba_devs[] __initdata = {
#if (CONFIG_HISILICON_KCONSOLE==1)
	& HIL_AMBADEV_NAME(uart1),
	& HIL_AMBADEV_NAME(uart0),
	& HIL_AMBADEV_NAME(uart2),
#elif (CONFIG_HISILICON_KCONSOLE==2)
	& HIL_AMBADEV_NAME(uart2),
	& HIL_AMBADEV_NAME(uart0),
	& HIL_AMBADEV_NAME(uart1),
#else
	& HIL_AMBADEV_NAME(uart0),
	& HIL_AMBADEV_NAME(uart1),
//	& HIL_AMBADEV_NAME(uart2),
#endif

};

extern int hisilicon_register_platform_devices(void);

static void __init hisilicon_init_machine(void)
{
	int i;

	edb_trace(1);


	for (i = 0; i < ARRAY_SIZE(amba_devs); i++) {
	        edb_trace(1);
		amba_device_register(amba_devs[i], &iomem_resource);
	}
	hisilicon_register_platform_devices();
	edb_trace(1);
}

#define REG_IOMAP(name) { \
		IO_ADDRESS(REG_BASE_##name),\
			  __phys_to_pfn(REG_BASE_##name), \
			   REG_##name##_IOSIZE, MT_DEVICE }

static struct map_desc hisilicon_io_desc[] __initdata = {
	REG_IOMAP( NANDC ),
	REG_IOMAP( SMI ),
	REG_IOMAP( SDIO ),
	REG_IOMAP( SIO0 ),
	REG_IOMAP( SIO1 ),
	REG_IOMAP( INTC ),
	REG_IOMAP( ETH ),	
	REG_IOMAP( USBOHCI ),
    REG_IOMAP( USBEHCI ),
    REG_IOMAP( CIPHER ),
    REG_IOMAP( DMAC ),
    REG_IOMAP( VEDU0 ),
    REG_IOMAP( VIU ),
    REG_IOMAP( TIMER12 ),
    REG_IOMAP( TIMER34 ),
    REG_IOMAP( WDG ),
    REG_IOMAP( SCTL ),
	REG_IOMAP( IR ),
	REG_IOMAP( UART0 ),
	REG_IOMAP( UART1 ),
	REG_IOMAP( UART2 ),
	REG_IOMAP( UART3 ),
	REG_IOMAP( I2C ),
	REG_IOMAP( SPI ),
	REG_IOMAP( IO ),
	REG_IOMAP( DDRC ),
	REG_IOMAP( VOU ),
	REG_IOMAP( TDE ),
	REG_IOMAP( GPIO0 ),
	REG_IOMAP( GPIO1 ),
	REG_IOMAP( GPIO2 ),
	REG_IOMAP( GPIO3 ),
	REG_IOMAP( GPIO4 ),
	REG_IOMAP( GPIO5 ),
	REG_IOMAP( GPIO6 ),
	REG_IOMAP( GPIO7 ),
};

static int hisilicon_iomapped = 0;

void __init hisilicon_map_io(void)
{
	int i;

	iotable_init(hisilicon_io_desc, ARRAY_SIZE(hisilicon_io_desc));

	hisilicon_iomapped = 1;

	edb_init();


	for(i=0; i<ARRAY_SIZE(hisilicon_io_desc); i++)
	{
#if (CONFIG_HISILICON_EDB_LEVEL == 0)
		edb_putstr(" V: "),	edb_puthex(hisilicon_io_desc[i].virtual);
		edb_putstr(" P: "),	edb_puthex(hisilicon_io_desc[i].pfn);
		edb_putstr(" S: "),	edb_puthex(hisilicon_io_desc[i].length);
		edb_putstr(" T: "),	edb_putul(hisilicon_io_desc[i].type);
		edb_putstr("\n");
#endif
		if(hisilicon_io_desc[i].virtual == IO_ADDRESS(~0)) {
			edb_putstr("<Bad IoTable> P: ");
			edb_puthex(hisilicon_io_desc[i].pfn);
			edb_putstr(" V: "); 
			edb_puthex(hisilicon_io_desc[i].virtual);
			edb_putstr("\n");
		}
	}

	edb_trace(1);
}

/* HI-hi3515v100 TIMER0 USING BUS CLK */
unsigned long hisilicon_timer_reload = BUSCLK_TO_TIMER_RELOAD(CONFIG_DEFAULT_BUSCLK);
unsigned long timer0_clk_hz = BUSCLK_TO_TIMER0_CLK_HZ(CONFIG_DEFAULT_BUSCLK);
unsigned long timer1_clk_hz = BUSCLK_TO_TIMER1_CLK_HZ(CONFIG_DEFAULT_BUSCLK);
unsigned long timer0_clk_khz = BUSCLK_TO_TIMER0_CLK_KHZ(CONFIG_DEFAULT_BUSCLK);
unsigned long timer1_clk_khz = BUSCLK_TO_TIMER1_CLK_KHZ(CONFIG_DEFAULT_BUSCLK);

unsigned long get_ahbclk_hw(void);

/* cycles to nsec conversions 
 * convert from cycles(64bits) => nanoseconds (64bits)
 *  basic equation:
 *		ns = cycles / (freq / ns_per_sec)
 *		ns = cycles * (ns_per_sec / freq)
 *		ns = cycles * (10^9 / (cpu_khz * 10^3))
 *		ns = cycles * (10^6 / cpu_khz)
 *
 *	Then we use scaling math (suggested by george at mvista.com) to get:
 *		ns = cycles * (10^6 * SC / cpu_khz)
 *		ns = cycles * cyc2ns_scale / SC
 *      Now we define SC = 2^10
 */
static unsigned long long hisi_cycles_2_ns(unsigned long long cyc)
{
	unsigned long cyc2ns_scale = (1000000 << 10)/timer1_clk_khz;
	return (cyc * cyc2ns_scale) >> 10;
}

static void timer_set_mode(enum clock_event_mode mode,
			   struct clock_event_device *clk)
{
	unsigned long ctrl;
	switch(mode) {
	case CLOCK_EVT_MODE_PERIODIC:
		writel(0, CFG_TIMER_VABASE + REG_TIMER_CONTROL);
	        writel(hisilicon_timer_reload, CFG_TIMER_VABASE + REG_TIMER_RELOAD);
	        writel(CFG_TIMER_CONTROL, CFG_TIMER_VABASE + REG_TIMER_CONTROL);
		edb_trace(1);
		break;
	case CLOCK_EVT_MODE_ONESHOT:
		writel(0, CFG_TIMER_VABASE + REG_TIMER_CONTROL);
		break;
	case CLOCK_EVT_MODE_UNUSED:
	case CLOCK_EVT_MODE_SHUTDOWN:
	default:
		ctrl = 0;
		writel(ctrl, CFG_TIMER_VABASE + REG_TIMER_CONTROL);
	}

}

static int timer_set_next_event(unsigned long evt,
				struct clock_event_device *unused)
{
	edb_trace(1);
	writel(evt, CFG_TIMER_VABASE + REG_TIMER_RELOAD);
        writel(CFG_TIMER_ONE_CONTROL, CFG_TIMER_VABASE + REG_TIMER_CONTROL);
	
	return 0;
}

static struct clock_event_device hisi_clockevent ={
	.name		= "hisi_timer0",
        .shift	        = 32,
	.features       = CLOCK_EVT_FEAT_PERIODIC | CLOCK_EVT_FEAT_ONESHOT,
	.set_mode	= timer_set_mode,
	.set_next_event	= timer_set_next_event,
};

/*
 * IRQ handler for systimer and hrtimer
 */
static irqreturn_t hisilicon_systimer_interrupt(int irq, void *dev_id)
{
	
	if((readl(CFG_TIMER_VABASE+REG_TIMER_RIS))&0x1)
	{
		writel(~0, CFG_TIMER_VABASE + REG_TIMER_INTCLR);
		hisi_clockevent.event_handler(&hisi_clockevent);
	}
	
	return IRQ_HANDLED;
}

static unsigned long free_timer_overflows=0;

static irqreturn_t hisilicon_freetimer_interrupt(int irq, void *dev_id)
{
	if((readl(CFG_TIMER_VABASE+REG_TIMER1_RIS))&0x1)
	{
		free_timer_overflows++;
		writel(~0, CFG_TIMER_VABASE + REG_TIMER1_INTCLR);
        }
	return IRQ_HANDLED;
}

static struct irqaction hisilicon_timer_irq = 
{
	.name		= "System Timer and Hrtimer",
	.flags		= IRQF_SHARED | IRQF_DISABLED | IRQF_TIMER,
	.handler	= hisilicon_systimer_interrupt,
};

static struct irqaction hisilicon_freetimer_irq = 
{
	.name		= "Free Timer",
	.flags		= IRQF_SHARED | IRQF_TIMER,
	.handler	= hisilicon_freetimer_interrupt,
};

static cycle_t hisi_get_cycles(void)
{
	//edb_trace(0);
	
	return ~readl(CFG_TIMER_VABASE + REG_TIMER1_VALUE);
}

/*
 * ---------------------------------------------------------------------------
 * hisi timer 1 ... free running 32-bit clock source and scheduler clock
 * ---------------------------------------------------------------------------
 */
static struct clocksource hisi_clocksource = {
	.name 		= "hisi_timer1",
 	.rating		= 200,
 	.read		= hisi_get_cycles,
	.mask		= CLOCKSOURCE_MASK(32),
 	.shift 		= 20,
	.flags		= CLOCK_SOURCE_IS_CONTINUOUS,
};

void hisilicon_timer_init(void)
{
	
	edb_trace(1);
	ulong busclk;
	
	/*Init timer1 is periodic running*/
	writel(0, CFG_TIMER_VABASE + REG_TIMER1_CONTROL);
	writel(0xffffffff, CFG_TIMER_VABASE + REG_TIMER1_RELOAD);
	writel(CFG_TIMER_CONTROL,CFG_TIMER_VABASE + REG_TIMER1_CONTROL);
	
	setup_irq(INTNR_TIMER_0_1, &hisilicon_timer_irq);
	setup_irq(INTNR_TIMER_0_1, &hisilicon_freetimer_irq);
	
	busclk = get_ahbclk_hw();
	hisilicon_timer_reload = BUSCLK_TO_TIMER_RELOAD(busclk);
	timer0_clk_hz = BUSCLK_TO_TIMER0_CLK_HZ(busclk);
	timer0_clk_khz = BUSCLK_TO_TIMER0_CLK_KHZ(busclk);
	timer1_clk_hz = BUSCLK_TO_TIMER1_CLK_HZ(busclk);
	timer1_clk_khz = BUSCLK_TO_TIMER1_CLK_KHZ(busclk);
	
	hisi_clocksource.mult = clocksource_khz2mult(timer1_clk_khz, hisi_clocksource.shift);
	clocksource_register(&hisi_clocksource);
 
	hisi_clockevent.mult =
		div_sc(timer0_clk_hz, NSEC_PER_SEC, hisi_clockevent.shift);	
    	hisi_clockevent.max_delta_ns =
		clockevent_delta2ns(0xffffffff, &hisi_clockevent);	
	hisi_clockevent.min_delta_ns =
		clockevent_delta2ns(0xf, &hisi_clockevent);
	hisi_clockevent.cpumask = cpumask_of_cpu(0);

	clockevents_register_device(&hisi_clockevent);	
}

unsigned long long sched_clock(void)
{
	unsigned long long ticks64;
	unsigned long ticks2,ticks1;

	ticks2 = 0UL - (unsigned long)readl(CFG_TIMER_VABASE + REG_TIMER1_VALUE);
	do{
		ticks1 = ticks2;
		ticks64 = free_timer_overflows;
		ticks2 = 0UL - (unsigned long)readl(CFG_TIMER_VABASE + REG_TIMER1_VALUE);
	}while(ticks1 > ticks2);

    /* 
    ** If INT is not cleaned, means the function is called with irq_save.
    ** The ticks has overflow but 'free_timer_overflows' is not be update.
    */
    if(readl(CFG_TIMER_VABASE + REG_TIMER1_MIS))
    {
        ticks64 += 1;
        ticks2 = 0UL - (unsigned long)readl(CFG_TIMER_VABASE + REG_TIMER1_VALUE);
    }
    
	return hisi_cycles_2_ns( (ticks64 << 32) | ticks2);
}
EXPORT_SYMBOL(sched_clock);

static struct sys_timer hisilicon_timer = 
{
	.init	= hisilicon_timer_init,
};

MACHINE_START(hi3515v100, "hi3515v100")
	.phys_io	= IO_SPACE_PHYS_START,
	.io_pg_offst	= (IO_ADDRESS(IO_SPACE_PHYS_START) >> 18) & 0xfffc, 
	.boot_params	= PHYS_OFFSET + 0x100,
	.map_io		= hisilicon_map_io,
	.init_irq	= hisilicon_init_irq,
	.timer		= &hisilicon_timer,
	.init_machine	= hisilicon_init_machine,
MACHINE_END

