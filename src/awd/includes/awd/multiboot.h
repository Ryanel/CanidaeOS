#pragma once

#include <stdint.h>
#include <multiboot.h>

#include <awd/kernel.h>

void multiboot_verify(uint32_t magic);
kernel_info_t multiboot_analyse(multiboot_info_t* mb_struct);