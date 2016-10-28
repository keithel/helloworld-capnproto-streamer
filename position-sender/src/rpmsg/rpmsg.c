
/*******************************************************************************
 *
 *            Copyright(c) 2015 L-3 Communication Corporation,
 *                 Warrior Systems Division, All Rights Reserved
 *
 *                THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF
 *           L-3 COMMUNICATIONS CORPORATION, WARRIOR SYSTEMS DIVISION
 *
 *    The copyright notice above does not evidence any actual or intended
 *    publication of such source code. This source code may contain
 *    technical data that is subject to the control of the international
 *    traffic in arms regulations and/or the export administration
 *    regulations.
 *
 *    L-3 Communications Corporation, Warrior Systems Division is currently
 *    determining the proper export jurisdiction of all of our components.
 *    You should treat all attached information as being Technical Data
 *    controlled under the International Traffic in Arms Regulations (ITAR)
 *    Parts 120-130, unless indicated to the contrary.  U.S. Government
 *    authorization is required to transfer Technical Data to a non U.S.
 *    person.  If you wish to export this information, contact Warrior
 *    Systems at 866-509-2040 (Export Compliance) for proper jurisdiction.
 *
 *****************************************************************************/
/**
 *  @file      rpmsg.c
 *  @date      Nov 11, 2015
 *
 *             Name           Initials
 *  @author    withee_j        <initials>
 *
 ******************************************************************************
 *  @brief
 *  <brief description goes here>
 *
 *  @detail
 *  <long description goes here>
 *
 ******************************************************************************
 *  @note
 *  <enter notes here>
 *
 *  @bug
 *  <None reported> or <enter any known bugs here>
 *
 *  @ingroup
 *  <module_group_name>
 ******************************************************************************/
/*----------------------------------------------------------------------------*/
/* Includes                                                                   */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "rpmsg.h"

/*----------------------------------------------------------------------------*/
/* Private Defines & Macros                                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private typedef                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private (static) Function Prototypes                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public (global) Variables                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private (static) Variables                                                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Functions Definitions                                               */
/*----------------------------------------------------------------------------*/
/**  
 *   @defgroup <group_name>  <group_description>
 *   @ingroup <module_group_name>
 */

int Rpmsg_OpenDevice( const char* dev )
{
   int fd; /* File description */

   if ( dev == NULL ) {
      return -1;
   }

   fd = open(dev, O_RDWR);
   if (fd < 0) {
      perror(__FUNCTION__);
      return -1;
   }

   return ( fd );
}

int Rpmsg_CloseDevice( int fd )
{
   int ret;

   ret = close( fd );
   if (ret < 0) {
      perror(__FUNCTION__);
      return -1;
   }
   return 0;
}

int Rpmsg_RecvMsg( int fd, void * data, size_t len )
{
   int retVal = 0;

   if ( data != NULL )
   {
      retVal = read( fd, data, len );
   }
   else
   {
      retVal = -1;
   }

   return ( retVal );
}

/* Not checking return state but it can be done */
int Rpmsg_SendMsg( int fd, IpcMsg_e command, void * data, size_t len )
{
   uint8_t buf[512];
   uint32_t ipcCommand = (uint32_t) command;
   int retVal = 0;

   memcpy(buf, &ipcCommand, sizeof(uint32_t));
   /*FIXME: bounds checking */
   memcpy(buf+sizeof(uint32_t), data, len );

   if ( write(fd, buf, sizeof(uint32_t) + len ) < 0 )
   {
      perror("Write");
      retVal = -1;
   }

   /* Read as much as we can, but verify that we only get one word and it is the ACK */
   if ( read( fd, buf, sizeof(buf) ) != sizeof(uint32_t) && (*((uint32_t* )buf) == 0xAAAAAAAA) )
   {
      retVal = -1;
   }

   return ( retVal );
}

/* Not checking return state but it can be done */
int Rpmsg_SendMsgNoAck( int fd, IpcMsg_e command, void * data, size_t len )
{
   uint8_t buf[512];
   uint32_t ipcCommand = (uint32_t) command;
   int retVal = 0;

   memcpy(buf, &ipcCommand, sizeof(uint32_t));
   /*FIXME: bounds checking */
   memcpy(buf+sizeof(uint32_t), data, len );

   if ( write(fd, buf, sizeof(uint32_t) + len ) < 0 )
   {
      perror("Write");
      retVal = -1;
   }

   return ( retVal );
}

/*----------------------------------------------------------------------------*/
/* Private (static) Function Definitions                                      */
/*----------------------------------------------------------------------------*/
/**
 *   @defgroup <group_name>  <group_description>
 *   @ingroup <module_group_name>
 */
/*  None  */


/* end rpmsg.c */
