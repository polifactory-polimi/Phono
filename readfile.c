#include <m_pd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILE_PATH "/sys/bus/iio/devices/iio:device0/in_voltage"

static t_class *readfile_class;

typedef struct _readfile {
    t_object x_obj;
    t_int filenum;
    t_outlet *f_out, *b_out;
} t_readfile;

t_float reader(t_int);
void *readfile_new(t_floatarg);

void readfile_bang(t_readfile *x)
{
    outlet_float(x->f_out, reader(x->filenum));
    outlet_bang(x->b_out);
}

void readfile_setup(void)
{
    readfile_class = class_new(gensym("readfile"),
        (t_newmethod)readfile_new,
        0, sizeof(t_readfile),
        CLASS_DEFAULT,
        A_DEFFLOAT, 0);
    class_addbang(readfile_class, readfile_bang);
}

void *readfile_new(t_floatarg num)
{
    t_readfile *x = (t_readfile *)pd_new(readfile_class);
    x->filenum = num;
    x->f_out = outlet_new(&x->x_obj, &s_float);
    x->b_out = outlet_new(&x->x_obj, &s_bang);
    return (void *)x;
}

t_float reader(t_int filenum)
{
    char path[64];
    char filename[16];
    sprintf(filename, "%d", (int)filenum);
    //concatenating path string + filename
    strcpy(path, FILE_PATH);
    strcat(path, filename);
    strcat(path, "_raw");
    char buf[16];
    FILE *fp;
    if(!(fp = fopen(path, "r"))) {
        printf("Error opening file %s", FILE_PATH);
        return 0;
    }
    fscanf(fp, "%s", buf);
    fclose(fp);
    return atoi(buf);
}
