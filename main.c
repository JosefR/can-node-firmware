
#define GPIOB_BASE 0x48000400
#define GPIO_MODER_OFFSET 0x00 // mode register offset
#define GPIO_ODR 0x14 // GPIO output data register offset
#define GPIOB_MODER (GPIOB_BASE + GPIO_MODER_OFFSET) // GPIO B mode register
#define GPUIB_ODR (GPIOB_BASE + GPIO_ODR) // GPIO B output data register

void clock_init()
{
    // just use the default internal RC oscillator for now


}

void gpio_init()
{
    *(GPIO_B_MODER) |= (0x50000000); // GPIO B14 and B15 are outputs

}


int main()
{
    clock_init();

    gpio_init();

    int i;
    while (1) {
        i++;
    }
}
