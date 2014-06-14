/* linux/arch/arm/mach-hi3515_v100/cpu-freq.c
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

#include <linux/config.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/init.h>

#include <linux/cpufreq.h>

#include <linux/clk.h>
#include <asm/hardware.h>

#include <asm/arch/system.h>
#include <asm/arch/clock.h>
#include <asm/arch/early-debug.h>

static int hisilicon_verify_freq(struct cpufreq_policy *policy)
{
	hisilicon_trace(4,"policy->min %u, policy->max %u", policy->min, policy->max);

	if (policy->cpu)
		return -EINVAL;

	cpufreq_verify_within_limits(policy, policy->cpuinfo.min_freq, policy->cpuinfo.max_freq);

	return 0;
}

static unsigned int hisilicon_get_freq(unsigned int cpu)
{
	struct clk *clk;
	unsigned int freq;

	if (cpu)
		return 0;

	clk = clk_get(NULL, "ARMCORECLK");
	if(clk == NULL)
		return 0;

	freq = clk_get_rate(clk)/KHZ;

	hisilicon_trace(4,"cur_freq = %u", freq);

	return freq;
}

#define CPU_FREQ_ALIGN_H(freq,align) ((((freq)+(align)-1)/(align))*(align))
#define CPU_FREQ_ALIGN_L(freq,align) (((freq)/(align))*(align))

static unsigned int calc_cpu_target_freq(struct cpufreq_policy *policy,
			 unsigned int target_freq,
			 unsigned int relation)
{
	if(strcmp(policy->governor->name, "userspace") ==0) {
		if(target_freq > policy->cpuinfo.max_freq)
			printk(KERN_WARNING "Warning: %uMHZ > %uMHZ, cpu-freq may overlade!\n",
					target_freq/KHZ ,policy->cpuinfo.max_freq/KHZ);
		if(target_freq < policy->cpuinfo.min_freq)
			printk(KERN_WARNING "Warning: %uMHZ < %uMHZ, cpu-freq underflow!\n",
					target_freq/KHZ ,policy->cpuinfo.min_freq/KHZ);
		return target_freq;
	}

	switch(relation)
	{
	case CPUFREQ_RELATION_L:
		target_freq = CPU_FREQ_ALIGN_H(target_freq, HISILICON_OSC_CLOCK/(9*KHZ));
		break;

	case CPUFREQ_RELATION_H:
		target_freq = CPU_FREQ_ALIGN_L(target_freq, HISILICON_OSC_CLOCK/(9*KHZ));
		break;
	}

	if(target_freq > policy->cpuinfo.max_freq)
		target_freq = policy->cpuinfo.max_freq;
	if(target_freq < policy->cpuinfo.min_freq)
		target_freq = policy->cpuinfo.min_freq;

	return target_freq;
}

static int hisilicon_cpu_target(struct cpufreq_policy *policy,
			 unsigned int target_freq,
			 unsigned int relation)
{
	struct clk *clk = clk_get(NULL, "ARMCORECLK");
	struct cpufreq_freqs freqs;

	if(clk==NULL)
		return -EIO;

	hisilicon_trace(4,"target_freq %u", target_freq);

	target_freq = calc_cpu_target_freq(policy, target_freq, relation);

	freqs.old = hisilicon_get_freq(0);
	freqs.new = target_freq;
	freqs.cpu = 0;

	cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);

	clk_set_rate(clk, target_freq*KHZ);
	freqs.new = hisilicon_get_freq(0);

	cpufreq_notify_transition(&freqs, CPUFREQ_POSTCHANGE);

	return 0;
}


static int __init hisilicon_cpu_init(struct cpufreq_policy *policy)
{
	struct clk *clk;

	hisilicon_trace(4,"policy->cpu %u", policy->cpu);

	if (policy->cpu != 0)
		return -EINVAL;
	clk = clk_get(NULL, "ARMCORECLK");
	if(clk == NULL)
	{
		printk(KERN_WARNING "ARMCORECLK not found!");
		return -1;
	}

	policy->governor = CPUFREQ_DEFAULT_GOVERNOR;

	policy->cpuinfo.min_freq = AHBCLK_TO_ARMCORE(20*MHZ)/KHZ;
	policy->cpuinfo.max_freq = AHBCLK_TO_ARMCORE(HISILICON_OSC_CLOCK*4)/KHZ;
	policy->cpuinfo.transition_latency = 500*1000;

	policy->cur = clk_get_rate(clk)/KHZ;
	policy->min = policy->cpuinfo.min_freq;
	policy->max = policy->cpuinfo.max_freq;

	hisilicon_trace(4,"policy->cur %u", policy->cur);

	return 0;
}

static struct cpufreq_driver hisilicon_cpu_driver = {
	.flags		= CPUFREQ_STICKY,
	.verify		= hisilicon_verify_freq,
	.target		= hisilicon_cpu_target,
	.get		= hisilicon_get_freq,
	.init		= hisilicon_cpu_init,
	.name		= "Hisilicon SOC",
};

static int __init hisilicon_cpufreq_init(void)
{
	printk(KERN_INFO "Hisilicon CPU-Freq\n");

	return cpufreq_register_driver(&hisilicon_cpu_driver);
}

module_init(hisilicon_cpufreq_init);

