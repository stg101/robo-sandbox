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
    printf("gaaaa\n");
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

    printf("firmware_path = %s\n", document["firmware_path"].GetString());

    return 0;
}