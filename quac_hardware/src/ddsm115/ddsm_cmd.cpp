#include <cstddef>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ddsm115/DDSM115CMD.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc < 3)  { printf("Usage: ros2 run quac ddsm_cmd <device> <cmd> [<options>]\n"); return 0; }

    DDSM115CMD cmd;
    if (cmd.connect(argv[1]) == false) {printf(cmd.get_error()); return 1;}
    

    if (strcmp(argv[2], "drive") == 0)
    {
        if (argc < 5) printf("Usage: ros2 run quac ddsm_cmd <device> drive <id> <rad/s>\n");
        else
        {
            uint8_t id = 0, mode = 0, err = 0;
            double cur = 0, vel = 0, pos = 0;
            if (cmd.drive(std::stoi(argv[3]), std::stof(argv[4]), 1, 0) == false) printf(cmd.get_error());
            usleep(20000);
            
            if (cmd.drive_feedback(&id, &mode, &cur, &vel, &pos, &err) == false) printf(cmd.get_error());
            else printf("cur %f vel %f pos %f\n", cur, vel ,pos);
        }
    }

    else if (strcmp(argv[2], "set_mode") == 0)
    {
        if (argc < 5) printf("Usage: ros2 run quac ddsm_cmd <device> set_mode <id> <mode ( cur | vel | pos )>\n");
        else
        {
            int mode = 0;
            if (strcmp(argv[4], "cur") == 0) mode = DRIVE_MODE_CURRENT;
            if (strcmp(argv[4], "vel") == 0) mode = DRIVE_MODE_VELOCITY;
            if (strcmp(argv[4], "pos") == 0) mode = DRIVE_MODE_POSITION;
            if (mode != 0)
            {
                if (cmd.set_mode(std::stoi(argv[3]), mode) == false) printf(cmd.get_error()); 
            }
            else
            {
                printf("Invalid argument '%s'\n", argv[4]);
                printf("Usage: ros2 run quac ddsm_cmd <device> set_mode <id> <mode ( cur | vel | pos )>\n");
            }
        }
    }
    else if (strcmp(argv[2], "set_id") == 0)
    {
        if (argc < 4) printf("Usage: ros2 run quac ddsm_cmd <device> set_id <id>\n");
        else
        {
            if (cmd.set_id(std::stoi(argv[3])) == false) printf(cmd.get_error());
        }
    }
    else
    {
        printf("Usage: ros2 run quac ddsm_cmd <device> <cmd> [<options>]\n");
    }

    cmd.disconnect();
}