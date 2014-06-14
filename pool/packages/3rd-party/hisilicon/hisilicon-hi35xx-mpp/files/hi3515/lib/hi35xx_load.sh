#!/bin/sh
# Useage: load [ -r ] [ -i ]
#         -r : rmmod all modules
#         -i : insmod all modules
#    default : rmmod all moules and then insmod them
#
# It is release load script.

insert_ko()
{
    insmod hi35xx_mmz.ko mmz=anonymous,0,0xC5000000,48M anony=1;
    insmod hi35xx_hidmac.ko
    insmod hi35xx_gpioi2c.ko
    #insmod hi35xx_tw2865.ko chips=2
    insmod hi35xx_fvideo.ko
    insmod hi35xx_nvp1114a.ko norm=2
    insmod hi35xx_hifb.ko video="hifb:vram0_size=1000000,vram1_size=800000,vram2_size=800000,vram3_size=800000,vram4_size=800000"
    insmod hi35xx_tde.ko
    insmod hi35xx_base.ko
    insmod hi35xx_sys.ko
    insmod hi35xx_dsu.ko
    insmod hi35xx_vd.ko
    insmod hi35xx_vou.ko
    insmod hi35xx_viu.ko
    insmod hi35xx_chnl.ko
    insmod hi35xx_vdec.ko nc_alg=1
    insmod hi35xx_venc.ko
    insmod hi35xx_group.ko
    insmod hi35xx_vpp.ko
    insmod hi35xx_md.ko
    insmod hi35xx_h264e.ko
    insmod hi35xx_h264d.ko
    insmod hi35xx_jpege.ko
    insmod hi35xx_jpegd.ko
    insmod hi35xx_sio.ko
    insmod hi35xx_ai.ko
    insmod hi35xx_ao.ko
    insmod hi35xx_aenc.ko
    insmod hi35xx_adec.ko

    insmod libphy.ko 
    insmod hi35xx_hiether.ko phyaddru=2
}

remove_ko()
{
    rmmod hi35xx_hiether
    rmmod libphy.ko 

    rmmod hi35xx_adec
    rmmod hi35xx_aenc
    rmmod hi35xx_ai
    rmmod hi35xx_ao
    rmmod hi35xx_sio
    rmmod hi35xx_jpegd
    rmmod hi35xx_jpege
    rmmod hi35xx_h264d
    rmmod hi35xx_h264e
    rmmod hi35xx_md
    rmmod hi35xx_vpp
    rmmod hi35xx_vdec
    rmmod hi35xx_venc
    rmmod hi35xx_group
    rmmod hi35xx_chnl
    rmmod hi35xx_dsu
    rmmod hi35xx_vou
    rmmod hi35xx_vd
    rmmod hi35xx_viu
    rmmod hi35xx_sys
    rmmod hi35xx_base
    rmmod hi35xx_hifb
    rmmod hi35xx_tde
    #rmmod hi35xx_tw2865
    rmmod hi35xx_nvp1114a
    rmmod hi35xx_fvideo
    #rmmod hi35xx_adv_7441
    rmmod hi35xx_gpioi2c
    rmmod hi35xx_hidmac
    rmmod hi35xx_mmz
}

# Use "sh xxx" can execute script even it has no executable attribute.
# !!! Must execute before insert module. For pin-multiplexing is configed here.
sh /usr/lib/hi35xx_sysctl.sh

# load module.
if [ "$1" = "-i" ]
then
	insert_ko
fi

if [ "$1" = "-r" ]
then
	remove_ko
fi

if [ $# -eq 0 ]
then
	remove_ko
	insert_ko
fi

