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
	unsigned char buf[HID_BUF_SIZE];
	wchar_t wstr[MAX_STR];
	hid_device *handle_keychron, *handle_esp32;

	res = hid_init();
#if defined(__APPLE__) && HID_API_VERSION >= HID_API_MAKE_VERSION(0, 12, 0)
	// To work properly needs to be called before hid_open/hid_open_path after hid_init.
	// Best/recommended option - call it right after hid_init.
	hid_darwin_set_open_exclusive(0);
#endif

	handle_keychron = hid_open(0x3434, 0x0430, NULL);
	if (!handle_keychron) {
		printf("Unable to open keychron hid device: %ls\n", hid_error(handle_keychron));
		hid_exit();
 		return 1;
	}

	handle_esp32 = hid_open(0xac05, 0x0a82, NULL);
	if (!handle_esp32) {
		printf("Unable to open esp32 hid device: %ls\n", hid_error(handle_esp32));
		hid_exit();
 		return 1;
	}

	res = hid_get_manufacturer_string(handle_keychron, wstr, MAX_STR);
	printf("Manufacturer String: %ls\n", wstr);
	res = hid_get_product_string(handle_keychron, wstr, MAX_STR);
	printf("Product String: %ls\n", wstr);

	res = hid_get_manufacturer_string(handle_esp32, wstr, MAX_STR);
	printf("Manufacturer String: %ls\n", wstr);
	res = hid_get_product_string(handle_esp32, wstr, MAX_STR);
	printf("Product String: %ls\n", wstr);

  while (1){
    printf("Reading from esp32\n");
    res = hid_read(handle_esp32, buf, HID_BUF_SIZE);
    print_buf(buf);
    hid_write(handle_esp32, buf, HID_BUF_SIZE);
    printf("Reading from keychron\n");
    res = hid_read(handle_keychron, buf, HID_BUF_SIZE);
    print_buf(buf);
  }

	// Close the device
	hid_close(handle_keychron);
  hid_close(handle_esp32);

	// Finalize the hidapi library
	res = hid_exit();

	return 0;
}
