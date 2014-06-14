/*
 * drivers/serial/solos_serial.c
 *
 * Maintainer : Guido Barzini 
 *
 * Copyright (C) 2004 Conexant
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 */
#include <linux/config.h>

#ifdef CONFIG_MAGIC_SYSRQ
#define SUPPORT_SYSRQ
#endif

#include <linux/module.h>
#include <linux/tty.h>
#include <linux/serial.h>
#include <linux/sysrq.h>
#include <linux/console.h>
#include <linux/serial_core.h>
#include <linux/delay.h>

#include <asm/hardware.h>

/* Forward decl.s */
static irqreturn_t solos_uart_int(int irq, void *dev_id, struct pt_regs *regs);

/* uart_ops functions */
static unsigned int solos_uart_tx_empty(struct uart_port *port)
{
	return (CHMIO_VIRT(UART_STATUS) & CHARM_UART_TXFULL) ? 0 : TIOCSER_TEMT;
}

static void solos_uart_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
	/* This is your basic 3-wire serial port. None of these signals exist. */
}

static unsigned int solos_uart_get_mctrl(struct uart_port *port)
{
	/* Claim unimplemented signals asserted, as per Documentation/serial/driver */
	return TIOCM_CTS | TIOCM_DSR | TIOCM_CD;
}

static void solos_uart_stop_tx(struct uart_port *port, unsigned int tty_stop)
{
	/* Turn off Tx interrupts. The port lock is held at this point */
	CHMIO_VIRT(UART_CTRL) &= ~CHARM_UART_TX_INTEN;
}

static void solos_uart_start_tx(struct uart_port *port, unsigned int tty_start)
{
	/* Turn on Tx interrupts. The port lock is held at this point */
	CHMIO_VIRT(UART_CTRL) |= CHARM_UART_TX_INTEN;
}

static void solos_uart_stop_rx(struct uart_port *port)
{
	/* Turn on Rx interrupts. The port lock is held at this point */
	CHMIO_VIRT(UART_CTRL) |= CHARM_UART_RX_INTEN;
}

static void solos_uart_enable_ms(struct uart_port *port)
{
	/* Nope, you really can't hope to attach a modem to this */
}

static void solos_uart_break_ctl(struct uart_port *port, int ctl)
{
	unsigned long flags;
	spin_lock_irqsave(&port->lock, flags);
	if (ctl)
		CHMIO_VIRT(UART_CTRL) |= CHARM_UART_GENBREAK;
	else
		CHMIO_VIRT(UART_CTRL) &= ~CHARM_UART_GENBREAK;
	spin_unlock_irqrestore(&port->lock, flags);
}

static int solos_uart_startup(struct uart_port *port)
{
	request_irq(IRQ_UART, solos_uart_int, SA_INTERRUPT, "solos_uart", port);
	/* Set port up: Rx and Break interrupts on. The speed should already be correct. */
	CHMIO_VIRT(UART_CTRL) |=
	    (CHARM_UART_BR_INTEN | CHARM_UART_RX_INTEN | CHARM_UART_UARTEN);

	/* The serial core uses this as a cookie, so we should set it even though
	 * it doesn't mean much here. */
	port->mapbase = (long)&CHMIO_VIRT(UART_STATUS);

	/* Informational */
	port->irq = IRQ_UART;

	/* Keeps the serial_core calculations happy... */
	port->uartclk = 80000000;
	return 0;
}

static void solos_uart_shutdown(struct uart_port *port)
{
	solos_uart_stop_tx(port, 0);
	solos_uart_stop_rx(port);
	solos_uart_break_ctl(port, 0);
	free_irq(IRQ_UART, port);
}

static void solos_uart_set_termios(struct uart_port *port, struct termios *new,
				   struct termios *old)
{
	unsigned long bits, flags;
	int baud = uart_get_baud_rate(port, new, old, 9600, 38400);
	switch (baud) {
	case 9600:
		bits = CHARM_UART_BAUD_9600;
		break;
	case 19200:
		bits = CHARM_UART_BAUD_19200;
		break;
	case 31250:
		bits = CHARM_UART_BAUD_31250;
		break;
	case 38400:
		bits = CHARM_UART_BAUD_38400;
		break;
	default:
		bits = CHARM_UART_BAUD_38400;
		break;
	}
	spin_lock_irqsave(&port->lock, flags);
	CHMIO_VIRT(UART_CTRL) &= ~CHARM_UART_BAUD_MASK;
	CHMIO_VIRT(UART_CTRL) |= bits;
	spin_unlock_irqrestore(&port->lock, flags);
}

static const char *solos_uart_type(struct uart_port *port)
{
	return port->type == PORT_SOLOS ? "Solos UART" : NULL;
}

static void solos_uart_release_port(struct uart_port *port)
{
	/* Easy enough */
}

static int solos_uart_request_port(struct uart_port *port)
{
	return 0;		/* How can we fail? */
}

static void solos_uart_config_port(struct uart_port *port, int flags)
{
	if (flags & UART_CONFIG_TYPE)
		port->type = PORT_SOLOS;
}

static int solos_uart_verify_port(struct uart_port *port,
				  struct serial_struct *ser)
{
	if (ser->type != PORT_UNKNOWN && ser->type != PORT_SOLOS)
		return -EINVAL;
	return 0;
}

static struct uart_ops solos_uart_ops = {
	.tx_empty = solos_uart_tx_empty,
	.set_mctrl = solos_uart_set_mctrl,
	.get_mctrl = solos_uart_get_mctrl,
	.stop_tx = solos_uart_stop_tx,
	.start_tx = solos_uart_start_tx,
	.stop_rx = solos_uart_stop_rx,
	.enable_ms = solos_uart_enable_ms,
	.break_ctl = solos_uart_break_ctl,
	.startup = solos_uart_startup,
	.shutdown = solos_uart_shutdown,
	.set_termios = solos_uart_set_termios,
	.type = solos_uart_type,
	.release_port = solos_uart_release_port,
	.request_port = solos_uart_request_port,
	.config_port = solos_uart_config_port,
	.verify_port = solos_uart_verify_port
};

/* This is on-chip so there is only ever exactly one of them */
static struct uart_port solos_uart_port = {
	.ops = &solos_uart_ops,
	.line = 0,
	.irq = IRQ_UART,
	.membase = (void *)(CHM_BASE_VIRT + CHM_UART_TX),
	.mapbase = CHM_BASE_VIRT + CHM_UART_TX,
	.uartclk = 16 * 38400,
	.type = PORT_SOLOS
};

static inline void solos_uart_int_rx_chars(struct uart_port *port,
					   struct pt_regs *regs)
{
	struct tty_struct *tty = port->info->tty;
	unsigned char ch;
	unsigned long status = CHMIO_VIRT(UART_STATUS);

	/* Handle exceptional conditions */
	if (status & (CHARM_UART_BREAK | CHARM_UART_RXORUN)) {
		if (status & CHARM_UART_BREAK) {
			*tty->flip.flag_buf_ptr = TTY_BREAK;
			port->icount.brk++;
			uart_handle_break(port);
		}
		if (status & CHARM_UART_RXORUN) {
			*tty->flip.flag_buf_ptr = TTY_OVERRUN;
		}
		/* Those bits are write-to-clear */
		CHMIO_VIRT(UART_STATUS) = status;
	}

	/* While we can read, do so ! */
	while ((status = CHMIO_VIRT(UART_STATUS)) & CHARM_UART_RXNOTEMPTY) {
		/* If we are full, just stop reading */
		if (tty->flip.count >= TTY_FLIPBUF_SIZE)
			break;

		/* Get the char */
		ch = (unsigned char)CHMIO_VIRT(UART_RX);

#if defined(CONFIG_MAGIC_SYSRQ) && defined(CONFIG_SERIAL_SOLOS_CONSOLE)
		if (uart_handle_sysrq_char(port, ch, regs)) {
			continue;	/* We don't process this character. */
		}
#endif

		/* Store it */
		*tty->flip.char_buf_ptr = ch;
		*tty->flip.flag_buf_ptr = 0;
		port->icount.rx++;

		tty->flip.char_buf_ptr++;
		tty->flip.flag_buf_ptr++;
		tty->flip.count++;

	}

	tty_flip_buffer_push(tty);
}

static inline void solos_uart_int_tx_chars(struct uart_port *port)
{
	struct circ_buf *xmit = &port->info->xmit;

	/* Process out of band chars */
	if (port->x_char) {
		CHMIO_VIRT(UART_TX) = port->x_char;
		port->icount.tx++;
		port->x_char = 0;
		return;
	}

	/* Nothing to do ? */
	if (uart_circ_empty(xmit) || uart_tx_stopped(port)) {
		solos_uart_stop_tx(port, 0);
		return;
	}

	/* Send chars */
	while (!(CHMIO_VIRT(UART_STATUS) & CHARM_UART_TXFULL)) {
		CHMIO_VIRT(UART_TX) = xmit->buf[xmit->tail];
		xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
		port->icount.tx++;
		if (uart_circ_empty(xmit))
			break;
	}

	/* Wake up */
	if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
		uart_write_wakeup(port);

	/* Maybe we're done after all */
	if (uart_circ_empty(xmit)) {
		solos_uart_stop_tx(port, 0);
		return;
	}

	return;
}

irqreturn_t solos_uart_int(int irq, void *dev_id, struct pt_regs * regs)
{
	struct uart_port *port = (struct uart_port *)dev_id;

	spin_lock(&port->lock);

	/* Process any Rx chars first */
	solos_uart_int_rx_chars(port, regs);
	/* Then use any Tx space */
	solos_uart_int_tx_chars(port);

	spin_unlock(&port->lock);

	return IRQ_HANDLED;
}

#ifdef CONFIG_SERIAL_SOLOS_CONSOLE

static void solos_console_write(struct console *co, const char *s,
				unsigned int count)
{
	struct uart_port *port = &solos_uart_port;
	unsigned long previous;
	int i;

	/* Save current state */
	previous = CHMIO_VIRT(UART_CTRL);
	/* Disable Tx interrupts so this all goes out in one go */
	solos_uart_stop_tx(port, 0);

	/* Write all the chars */
	for (i = 0; i < count; i++) {

		/* Wait the TX buffer to be empty, which can't take forever:
		 * there's no flow control on the UART */
		while (CHMIO_VIRT(UART_STATUS) & CHARM_UART_TXFULL)
			udelay(1);

		/* Send the char */
		CHMIO_VIRT(UART_TX) = *s;

		/* CR/LF stuff */
		if (*s++ == '\n') {
			/* Wait the TX buffer to be empty */
			while (CHMIO_VIRT(UART_STATUS) & CHARM_UART_TXFULL)
				udelay(1);
			CHMIO_VIRT(UART_TX) = '\r';
		}
	}

	CHMIO_VIRT(UART_CTRL) = previous;	/* Put it all back */
}

static int __init solos_console_setup(struct console *co, char *options)
{
	struct uart_port *port = &solos_uart_port;
	int ret;
	int baud = CONFIG_SERIAL_SOLOS_CONSOLE_BAUD;
	int bits = 8;
	int parity = 'n';
	int flow = 'n';

	if (co->index != 0) {	/* not much choice, there's only one of these. */
		CHMIO_VIRT(UART_TX) = 'a';
		return -EINVAL;
	}
	/* This isn't going to do much, but it might change the baud rate. */
	if (options)
		uart_parse_options(options, &baud, &parity, &bits, &flow);
	CHMIO_VIRT(UART_TX) = 'b';
	ret = uart_set_options(port, co, baud, parity, bits, flow);
	CHMIO_VIRT(UART_TX) = 'X';
	return ret;
}

static struct uart_driver solos_uart_driver;	/* Forward decl. */

static struct console solos_console = {
	.name = "ttyS",
	.write = solos_console_write,
	.device = uart_console_device,
	.setup = solos_console_setup,
	.flags = CON_PRINTBUFFER,
	.index = -1,		/* Only possible option. */
	.data = &solos_uart_driver,
};

static int __init solos_console_init(void)
{
	solos_console.index = 0;
	register_console(&solos_console);
	return 0;
}

console_initcall(solos_console_init);

#endif

static struct uart_driver solos_uart_driver = {
	.owner = THIS_MODULE,
	.driver_name = "solos_uart",
	.dev_name = "ttyS",
	.devfs_name = "ttyS",
	.major = TTY_MAJOR,
	.minor = 64,
	.nr = 1,
#ifdef CONFIG_SERIAL_SOLOS_CONSOLE
	.cons = &solos_console,
#else
	.cons = NULL;
#endif
};

static int __init solos_uart_init(void)
{
	int ret;
	printk(KERN_INFO "Solos on-chip UART init.\n");
	solos_uart_port.ops = &solos_uart_ops;
	ret = uart_register_driver(&solos_uart_driver);
	if (ret == 0) {
		solos_uart_port.ops = &solos_uart_ops;
		solos_uart_port.line = 0;
		solos_uart_port.irq = IRQ_UART;
		solos_uart_port.membase = (void *)(CHM_BASE_VIRT + CHM_UART_TX);
		solos_uart_port.mapbase = CHM_BASE_VIRT + CHM_UART_TX;
		solos_uart_port.uartclk = 16 * 38400;
		ret = uart_add_one_port(&solos_uart_driver, &solos_uart_port);
	}
	return ret;
}

static void __exit solos_uart_exit(void)
{
	uart_unregister_driver(&solos_uart_driver);
}

module_init(solos_uart_init);
module_exit(solos_uart_exit);

MODULE_AUTHOR("Guido Barzini");
MODULE_DESCRIPTION("Conexant Solos UART driver");
MODULE_LICENSE("GPL");
