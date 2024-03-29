#include <pthread.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <wchar.h>

#include "hidapi.h"
#if defined(__APPLE__) && HID_API_VERSION >= HID_API_MAKE_VERSION(0, 12, 0)
#include <hidapi_darwin.h>
#endif

#define MAX_STR 255
#define HID_BUF_SIZE 32
#define KEYCHRON

void print_buf(unsigned char buf[])
{
  for (int i = 0; i < HID_BUF_SIZE; i++) {
    printf("%02X ", buf[i]);
  }
  printf("\n");
}

int main(int argc, char* argv[])
{
	int res;
	unsigned char buf[HID_BUF_SIZE] = {0};
	wchar_t wstr[MAX_STR];
	hid_device *handle_keychron, *handle_esp32;

	res = hid_init();
#if defined(__APPLE__) && HID_API_VERSION >= HID_API_MAKE_VERSION(0, 12, 0)
	hid_darwin_set_open_exclusive(0);
#endif

#ifdef KEYCHRON
  struct hid_device_info* keychron_interface = hid_enumerate(0x3434, 0x0430);
  do {
    if (keychron_interface->usage_page == 0xff60 && keychron_interface->usage == 0x61)
      break;
    else 
      keychron_interface = keychron_interface->next;
  } while (keychron_interface->next);

  if (keychron_interface->usage_page != 0xff60 || keychron_interface->usage != 0x61) {
    printf("Unable to find keychron hid device and interface");
  }

	handle_keychron = hid_open_path(keychron_interface->path);
	if (!handle_keychron) {
		printf("Unable to open keychron hid device: %ls\n", hid_error(handle_keychron));
		hid_exit();
 		return 1;
	}
#endif

	handle_esp32 = hid_open(0xac05, 0x0a82, NULL);
	if (!handle_esp32) {
		printf("Unable to open esp32 hid device: %ls\n", hid_error(handle_esp32));
		hid_exit();
 		return 1;
	}

#ifdef KEYCHRON
  res = hid_get_manufacturer_string(handle_keychron, wstr, MAX_STR);
	res = hid_get_manufacturer_string(handle_keychron, wstr, MAX_STR);
	printf("Manufacturer String: %ls\n", wstr);
	res = hid_get_product_string(handle_keychron, wstr, MAX_STR);
	printf("Product String: %ls\n", wstr);
#endif

	res = hid_get_manufacturer_string(handle_esp32, wstr, MAX_STR);
	printf("Manufacturer String: %ls\n", wstr);
	res = hid_get_product_string(handle_esp32, wstr, MAX_STR);
	printf("Product String: %ls\n", wstr);

  while (1){
    memset(buf, 0, HID_BUF_SIZE);
    res = hid_read(handle_esp32, buf, HID_BUF_SIZE);
    if (res == -1){
      printf("Fatal esp32: %ls", hid_error(handle_esp32));
      exit(-1);
    }
    printf("esp32[%d]: ", res);
    print_buf(buf);
  #ifdef KEYCHRON
    
    res = hid_write(handle_keychron, buf, HID_BUF_SIZE);
    if (res == -1){
      printf("Fatal keychron: %ls", hid_error(handle_keychron));
      exit(-1);
    }
  #endif
  }

#ifdef KEYCHRON
	hid_close(handle_keychron);
#endif
  hid_close(handle_esp32);

	res = hid_exit();

	return 0;
}
