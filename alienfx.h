#ifndef ALIENFX_H
#define ALIENFX_H

#include "alienfx.h"
#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <errno.h>


#define VENDOR_ID     0x187c
#define MY_PRODUCT_ID 0x0524

#define ALIENFX_USER_CONTROLS 0x01
#define ALIENFX_SLEEP_LIGHTS 0x02
#define ALIENFX_ALL_OFF 0x03
#define ALIENFX_ALL_ON 0x04

#define ALIENFX_MORPH 0x01
#define ALIENFX_BLINK 0x02
#define ALIENFX_STAY 0x03
#define ALIENFX_BATTERY_STATE 0x0F

#define ALIENFX_TOUCHPAD        0x000001
#define ALIENFX_LIGHTPIPE       0x000020
#define ALIENFX_ALIENWARE_LOGO  0x000080
#define ALIENFX_ALIENHEAD       0x000100
#define ALIENFX_POWER_BUTTON    0x008000
#define ALIENFX_TOUCH_PANEL     0x010000

#define ALIENFX_DEVICE_RESET 0x06
#define ALIENFX_READY 0x10
#define ALIENFX_BUSY 0x11
#define ALIENFX_UNKOWN_COMMAND 0x12

#define SEND_REQUEST_TYPE 0x21
#define SEND_REQUEST      0x09
#define SEND_VALUE        0x202
#define SEND_INDEX        0x00
#define SEND_DATA_SIZE    9

#define READ_REQUEST_TYPE 0xa1
#define READ_REQUEST      0x01
#define READ_VALUE        0x101
#define READ_INDEX        0x0
#define READ_DATA_SIZE    9

#define START_BYTE               	  0x02
#define FILL_BYTE                	  0x00

void usbTestCode(void);
int is_usbdevblock( libusb_device *dev );



bool WriteDevice(libusb_device_handle *usb_handle,
                 unsigned char *data,
                 int data_bytes){
    unsigned char buf[SEND_DATA_SIZE];
    memset(&buf[0], FILL_BYTE, SEND_DATA_SIZE);
    memcpy(&buf[0], data, data_bytes);
    int written_bytes = libusb_control_transfer(usb_handle,
                                                SEND_REQUEST_TYPE,
                                                SEND_REQUEST,
                                                SEND_VALUE,
                                                SEND_INDEX,
                                                buf, SEND_DATA_SIZE, 0);
    if(written_bytes != SEND_DATA_SIZE)
        fprintf(stderr,
                "WriteDevice: wrote %d bytes instead of expected %d %s\n",
                written_bytes, data_bytes, strerror(errno));

    printf("Write: %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x  sent:%i\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], written_bytes);

    return (written_bytes == SEND_DATA_SIZE);
}

void SetColor( libusb_device_handle *usb_handle, unsigned char pAction, unsigned char pSetId, unsigned int pLeds, unsigned int pColor){
  size_t BytesWritten;
  unsigned char Buffer[] = { 0x02 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 };
  printf("%.4x\n", pLeds );

  Buffer[1] = pAction;
  Buffer[2] = pSetId;
  Buffer[3] = 0x00;//(pLeds & 0xFF0000) >> 16;
  Buffer[4] = (pLeds & 0xFF00) >> 8   ;//  0x4B
  Buffer[5] = (pLeds & 0x00FF);//        0xEF
  Buffer[6] = (pColor & 0xFF00) >> 8;
  Buffer[7] = (pColor & 0x00FF);
  Buffer[8] = 0x00;//(pColor & 0x0000FF);

  WriteDevice(usb_handle, Buffer, 9);
}

void SetSpeed(libusb_device_handle *usb_handle){
  unsigned char Buffer[] = { 0x02 ,0x0E ,0x00 ,0xC8 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 };
  WriteDevice(usb_handle, Buffer, 9);
}

void Reset(libusb_device_handle *usb_handle, unsigned char pOptions){
  unsigned char Buffer[] = { 0x02 ,0x07 ,0x04 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 };
  WriteDevice(usb_handle, Buffer, 9);
}

void GetStatus(libusb_device_handle *usb_handle){
  unsigned char Buffer[] = { 0x02 ,0x06 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 };
  WriteDevice(usb_handle, Buffer, 9);
}

// return number of bytes read
int ReadDevice(libusb_device_handle *usb_device,
               unsigned char *data, // point to buffer to receive data
               int data_bytes)
{
    unsigned char buf[READ_DATA_SIZE];
    memset(&buf[0], FILL_BYTE, READ_DATA_SIZE);
    int read_bytes = 0 , i = 0;
    do {
         read_bytes = libusb_control_transfer(usb_device,
                                             READ_REQUEST_TYPE, READ_REQUEST,
                                             READ_VALUE, READ_INDEX,
                                             &buf[0], sizeof buf, data_bytes);
         i++;
    } while ((buf[0]!= ALIENFX_READY) && (i < 10));
    memcpy(data, &buf[0], read_bytes);
    printf("Read:  %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
    return read_bytes;
}

void EndLoopBlock(libusb_device_handle *usb_handle){
  size_t BytesWritten;
  unsigned char Buffer[] = { 0x02 ,0x04 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 };
  WriteDevice(usb_handle, Buffer, 9);
}

void EndTransmitionAndExecute(libusb_device_handle *usb_handle){
  size_t BytesWritten;
  unsigned char Buffer[] = { 0x02 ,0x05 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 };
  WriteDevice(usb_handle, Buffer, 9);
}

int is_usbdevblock( libusb_device *dev )
{
        struct libusb_device_descriptor desc;
        int r = libusb_get_device_descriptor( dev, &desc );

        if( desc.idVendor == VENDOR_ID && desc.idProduct == MY_PRODUCT_ID ){
                return 1;
        }

        return 0;
}

void usbTestCode(unsigned int region, unsigned int color){

    // discover devices
    libusb_device **list;
    libusb_device *found = NULL;
    libusb_context *ctx = NULL;
    int attached = 0;

    libusb_init(&ctx);
    libusb_set_debug(ctx,3);
    ssize_t cnt = libusb_get_device_list(ctx, &list);
    ssize_t i = 0;
    int err = 0;
    if (cnt < 0){
            printf( "no usb devices found\n" );
            return;
    }

    for(i = 0; i < cnt; i++){
            libusb_device *device = list[i];
            if( is_usbdevblock(device) ){
                    found = device;
                    break;
            }
    }

    if(found){
            printf( "found usb-dev-block!\n" );
            libusb_device_handle *handle;
            err = libusb_open(found, &handle);
            if (err){
                    printf("Unable to open usb device\n");
                    return;
            }

            if ( libusb_kernel_driver_active(handle,0) ){
                    printf("Device busy...detaching...\n");
                    libusb_detach_kernel_driver(handle,0);
                    attached = 1;
            }else printf("Device free from kernel\n");

            err = libusb_claim_interface( handle, 0 );
            if (err){
                    printf( "Failed to claim interface. " );
                    switch( err ){
                    case LIBUSB_ERROR_NOT_FOUND:    printf( "not found\n" );        break;
                    case LIBUSB_ERROR_BUSY:         printf( "busy\n" );             break;
                    case LIBUSB_ERROR_NO_DEVICE:    printf( "no device\n" );        break;
                    default:                        printf( "other\n" );            break;
                    }
                    return;
            }

            unsigned char data_input[READ_DATA_SIZE];

            //ReadDevice(handle, &data_input[0], READ_DATA_SIZE );
            Reset(handle, ALIENFX_ALL_ON);
            GetStatus(handle);

            ReadDevice(handle, &data_input[0], READ_DATA_SIZE );
            SetSpeed(handle);
            SetColor(handle, ALIENFX_STAY, 1 ,region,color);
            EndLoopBlock(handle);
            EndTransmitionAndExecute(handle);
            GetStatus(handle);
            //ReadDevice(handle, &data_input[0], READ_DATA_SIZE );




            //Clean this mess up!
            libusb_close(handle);
            libusb_exit(ctx);

    }


    return;
}

#endif // ALIENFX_H






