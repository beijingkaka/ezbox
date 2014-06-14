/* Copyright Conexant Systems, Inc. 2004
#
# Conexant Systems, Inc. Confidential and Proprietary
#
# The following software source code ("Software") is strictly confidential and
# is proprietary to Conexant Systems, Inc. and its subsidiaries (collectively,
# "Conexant"). It may only be read, used, copied, adapted, modified or 
# otherwise dealt with by you if you have entered into a license agreement 
# with Conexant and then subject to the terms of that agreement and any other 
# applicable confidentiality agreement between you and Conexant.
#
# If you are in any doubt as to whether you are entitled to access, read,
# use, copy, adapt, modify or otherwise deal with the Software or whether you
# are entitled to disclose the Software to any other person you should
# contact Conexant.  If you have not entered into a license agreement
# with Conexant granting access to this Software you should forthwith return
# all media, copies and printed listings containing the Software to Conexant.
#
# Conexant reserves the right to take legal action against you should you
# breach the above provisions.
#
# If you are unsure, or to report violations, please contact
# licensee.support@conexant.com
# ##########################################################################*/

/*******************************************************************************
*
*                              I N C L U D E   F I L E S
*
*******************************************************************************/
#include "asm-arm/arch-solos/hardware/solosw_gpio.h"
#include <asm-arm/semaphore.h>
#include <linux/errno.h>
/*******************************************************************************
*
*                             L O C A L   V A R I A B L E S
*
*******************************************************************************/
struct semaphore	solosw_gpio_sem;

/*******************************************************************************
*
*                              L O C A L   M E T H O D S
*
*******************************************************************************/
/*!
\brief Set operation mode for a GPIO
\param GPIONum    32-bit unsigned    INPUT
\param Mode     solosw_gpio_mode_t        INPUT
\return 32-bit unsigned error code:
  \retval SOLOSW_GPIO_SUCCESS: Success
  \retval SOLOSW_GPIO_INVGPIO: Illegal GPIO number
  \retval SOLOSW_GPIO_INVMODE: Illegal mode
\note
*/
U32 solosw_gpio_set_mode(U32 gpioNum, solosw_gpio_mode_t mode)
{
    U32 offset;
    PRINTK(KERN_WARNING "Solosw gpio set mode  %d: GPIO  %d \n", mode,gpioNum);

    if(gpioNum >= SOLOSW_GPIO_NUMS)
        return SOLOSW_GPIO_INVGPIO;
    
    if(mode < INPUT || mode > OPENSOURCE)
        return SOLOSW_GPIO_INVMODE;

    offset = SOLOSW_GPIO_OFFSET(CONTROL, gpioNum); 
    REGIO_GPIO(CONTROL, gpioNum) = mode << SOLOSW_GPIO_SHIFT(CONTROL, gpioNum);    

    return SOLOSW_GPIO_SUCCESS;
}

/*!
\brief Get operation mode of a GPIO
\param GPIONum    32-bit unsigned               INPUT
\param Mode       Pointer to 32-bit unsigned    OUTPUT 
\return 32-bit unsigned error code:
  \retval SOLOSW_GPIO_SUCCESS : Success
  \retval SOLOSW_GPIO_INVGPIO : Illegal GPIO number
  \retval SOLOSW_GPIO_INVARG  : Illegal "Mode" parameter (NULL pointer)
\note 
-# The caller must provide sizeof(U32) available bytes
              at address pointed to by "Mode". 
-# This function returns the mode field of the control
   register without checking whether it contains a valid mode.
*/
U32 solosw_gpio_get_mode(U32 gpioNum, U32* mode)
{
    U32 offset;

    if(gpioNum >= SOLOSW_GPIO_NUMS)
        return SOLOSW_GPIO_INVGPIO;
    
    if(mode == NULL)
        return SOLOSW_GPIO_INVARG;
    
    offset = SOLOSW_GPIO_OFFSET(CONTROL, gpioNum);

    *mode  =  (REGIO_GPIO(CONTROL, gpioNum)  >> SOLOSW_GPIO_SHIFT(CONTROL, gpioNum)) & SOLOSW_GPIO_CONTROL_MASK;

     PRINTK(KERN_WARNING "Solosw gpio get mode  %d: GPIO %d\n",*mode, gpioNum);	
       
     return SOLOSW_GPIO_SUCCESS;
}

/*!
\brief Assert an output pin (fast)
\param GPIONum    32-bit unsigned    INPUT
\return 
\note 
-# Inlined function.
-# Does not check that the pin has actually been programmed 
   as output.
-# Does not check validity of GPIONum.
-# Uses the set registers (0x250 and 0x254) and thus
   does not perform a read cycle before writing to the register.
-# Due to the above point there is no danger of a race condition
   and thus no critical section is needed.
*/
void solosw_gpio_assert_output(U32 gpioNum)
{
   PRINTK(KERN_WARNING "Solosw gpio assert GPIO %d\n", gpioNum);	

    if(gpioNum >= SOLOSW_GPIO_NUMS)
        return;

    REGIO_GPIO(SET, gpioNum) = 1 << SOLOSW_GPIO_SHIFT(SET, gpioNum);
}

/*!
\brief De-assert an output pin  (fast)
\param GPIONum    32-bit unsigned    INPUT
\return 
\note 
-# Inlined function.
-# Does not check that the pin has actually been programmed 
   as output.
-# Does not check validity of GPIONum.
-# Uses the clear registers (0x258 and 0x25c) and thus
   does not perform a read cycle before writing to the register.
-# Due to the above point there is no danger of a race condition
   and thus no critical section is needed.
*/
void solosw_gpio_deassert_output(U32 gpioNum)
{
   PRINTK(KERN_WARNING "Solosw gpio seassert GPIO %d\n", gpioNum);	

   if(gpioNum >= SOLOSW_GPIO_NUMS)
        return;

   REGIO_GPIO(CLEAR, gpioNum) = 1 << SOLOSW_GPIO_SHIFT(CLEAR, gpioNum);
}

/*!
\brief Assert an output pin 
\param 32-bit unsigned    GPIONum    INPUT
\return 
\note 
-# The OUTPUT register is accessed atomically by disabling IRQs.
-# Inlined function.
-# Does not check that the pin has actually been programmed 
   as output.
-# Does not check validity of GPIONum.
-# Uses the output registers (0x240 and 0x244) and thus
   performs a read cycle before writing to the register (read,
   or in the value, write back the result).
*/
void solosw_gpio_assert_output_slow(U32 gpioNum)
{
   PRINTK(KERN_WARNING "Solosw gpio assert out slow GPIO %d\n", gpioNum);	

   if(gpioNum >= SOLOSW_GPIO_NUMS)
        return ;

   if ( LOCK_SOLOSW_GPIO_FOR_WRITING )
	return;
    
  REGIO_GPIO(OUTPUT, gpioNum) |= 1 << SOLOSW_GPIO_SHIFT(OUTPUT, gpioNum);

  UNLOCK_SOLOSW_GPIO_FOR_WRITING;  
}

/*!
\brief De-assert an output pin 
\param GPIONum    32-bit unsigned    INPUT
\return 
\note  
-# The OUTPUT register is accessed atomically by disabling IRQs.
-# Inlined function.
-# Does not check that the pin has actually been programmed 
   as output.
-# Does not check validity of GPIONum.
-# Uses the output registers (0x240 and 0x244) and thus
   performs a read cycle before writing to the register (read,
   reset the bit, write back the result).
*/
void solosw_gpio_deassert_output_slow(U32 gpioNum)
{
   PRINTK(KERN_WARNING "Solosw gpio deassert out slow GPIO %d\n", gpioNum);	

   if(gpioNum >= SOLOSW_GPIO_NUMS)
        return;

   if ( LOCK_SOLOSW_GPIO_FOR_WRITING )
        return;
  
   REGIO_GPIO(OUTPUT, gpioNum) &= ~(1 << SOLOSW_GPIO_SHIFT(OUTPUT, gpioNum));

   UNLOCK_SOLOSW_GPIO_FOR_WRITING;
}

/*!
\brief Check the state of an input pin 
\param 32-bit unsigned    GPIONum    INPUT

\note 
-# Inlined function.
-# Does not check that the pin has actually been programmed 
   as input.
-# Does not check validity of GPIONum.
*/
U32 solosw_gpio_check_input(U32 gpioNum)
{
    U32 val;

    if(gpioNum >= SOLOSW_GPIO_NUMS)
        return SOLOSW_GPIO_INVGPIO;

   val =  (REGIO_GPIO(INPUT, gpioNum) & (1 << SOLOSW_GPIO_SHIFT(INPUT, gpioNum))) >> SOLOSW_GPIO_SHIFT(INPUT, gpioNum);
 
   PRINTK(KERN_WARNING "Solosw gpio read: GPIO %d %d\n", val, gpioNum);

   return val;	
}

