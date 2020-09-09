#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: recover [filename]\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    unsigned char buffer[512];


    //Counter for number of JPEGS
    int newJpeg = 0;

    //Enough characters for a file name in format "000.jpg"
    char filename[8];

    //File to be created for each JPEG
    FILE *img = NULL;

    //While we haven't reached the end of the memory card
    while (fread(buffer, 512, 1, file) == 1)
    {
        //If the header matches the start of a JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {

            //If it's not our first JPEG, then close the previously opened one before proceeding
            if(newJpeg != 0)
            {
                fclose(img);
            }


            sprintf(filename, "%03i.jpg", newJpeg);
            img = fopen(filename, "w");
            fwrite(buffer, 512, 1, img);
            newJpeg++;
        }
        //Else if it's not the start of a row of JPEGs
        else
        {
            if(img!= NULL) // Make sure the file is open first
            {
                fwrite(buffer, 512, 1, img);
            }
        }
    }
    fclose(img);
    fclose(file);
    return 0;
}
