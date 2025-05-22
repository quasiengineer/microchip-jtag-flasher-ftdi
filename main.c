#include <stdio.h>
#include <stdlib.h>
#include <ftdi.h>

#include "dpuser.h"
#include "dpalg.h"

void configure_adapter() {
  ftdi_init(&ftdi);

  if (ftdi_usb_open(&ftdi, 0x0403, 0x6014) < 0) {
    fprintf(stderr, "open error: %s\n", ftdi_get_error_string(&ftdi));
    exit(EXIT_FAILURE);
  }

  ftdi_usb_reset(&ftdi);
  ftdi_set_bitmode(&ftdi, 0b10001011, BITMODE_BITBANG); // TCK / TDI / TMS / TRST - output
}

void read_data_file(const char *filename) {
  FILE * file = fopen(filename, "rb");  // Open file in binary mode
  if (!file) {
    perror("Failed to open file");
    exit(EXIT_FAILURE);
  }

  fseek(file, 0, SEEK_END);
  long filesize = ftell(file);
  rewind(file);

  if (filesize <= 0) {
    printf("File is empty or error determining size.\n");
    fclose(file);
    exit(EXIT_FAILURE);
  }

  // Allocate buffer to hold the entire file
  unsigned char * buffer = (unsigned char *)malloc(filesize);
  if (!buffer) {
    perror("Memory allocation failed");
    fclose(file);
    exit(EXIT_FAILURE);
  }

  // Read the entire file into buffer
  size_t read_size = fread(buffer, 1, filesize, file);
  fclose(file);

  if (read_size != filesize) {
    printf("Warning: Read size differs from expected size.\n");
  }

  image_buffer = buffer;
}

int main(int argc, char * argv[]) {
  char * dataFilePath = argv[1];

  printf("Microchip JTAG Flasher\n");
  printf("Data file path: %s\n", dataFilePath);

  configure_adapter();
  read_data_file(dataFilePath);

  // execute action via DirectC
   Action_code = DP_PROGRAM_ACTION_CODE;
//  Action_code = DP_DEVICE_INFO_ACTION_CODE;
  dp_top();

  // newline
  puts("");

  free(image_buffer);
  ftdi_usb_close(&ftdi);
  ftdi_deinit(&ftdi);

  return 0;
}
