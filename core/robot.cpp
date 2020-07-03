#include "robot.h"

static bool sReadFile(char *&data, int &size, const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == nullptr)
    {
        return false;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (size == 0)
    {
        return false;
    }

    data = (char *)malloc(size + 1);
    fread(data, size, 1, file);
    fclose(file);
    data[size] = 0;

    return true;
}

int Robot::loadDefinition(const char *path)
{
    char *data = nullptr;
    int size = 0;
    bool found = sReadFile(data, size, path);
    if (found == false)
    {
        return 1;
    }

    rapidjson::Document document;
    if (document.Parse(data).HasParseError())
        return 1;

    const char *firmware_path = document["firmware"]["path"].GetString();
    const char *firmware_mcu = document["firmware"]["mcu"].GetString();
    int firmware_frecuency = document["firmware"]["frecuency"].GetInt();

    fname = firmware_path;
    avr = avr_make_mcu_by_name(firmware_mcu);

    if (!avr)
    {
        printf("mcu name not found\n");
        return -1;
    }
    avr_init(avr);

    avr_extint_set_strict_lvl_trig(avr, EXTINT_IRQ_OUT_INT0, 0);
    avr_extint_set_strict_lvl_trig(avr, EXTINT_IRQ_OUT_INT1, 0);
    {
        /* Load .hex and setup program counter */
        uint32_t boot_base, boot_size;
        uint8_t *boot = read_ihex_file(fname, &boot_size, &boot_base);
        if (!boot)
        {
            fprintf(stderr, "Unable to load %s\n", fname);
            return -1;
        }
        memcpy(avr->flash + boot_base, boot, boot_size);
        free(boot);
        avr->pc = boot_base;
        /* end of flash, remember we are writing /code/ */
        avr->codeend = avr->flashend;
    }

    /* more simulation parameters */
    avr->frequency = firmware_frecuency;
    avr->aref = ADC_VREF_V256;
    avr->sleep = sleepHook;

    return 0;
}

void Robot::sleepHook(
    avr_t *avr,
    avr_cycle_count_t how_long)
{
    return;
}