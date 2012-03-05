//Imlibtest.c

/* main program */
int main(int argc, char **argv)
{
  /* an image handle */
  Imlib_Image image;
  
  /* if we provided < 2 arguments after the command - exit */
  if (argc != 3) exit(1);
  /* load the image */
  image = imlib_load_image(argv[1]);
  /* if the load was successful */
  if (image)
    {
      char *tmp;
      /* set the image we loaded as the current context image to work on */
      imlib_context_set_image(image);
      /* set the image format to be the format of the extension of our last */
      /* argument - i.e. .png = png, .tif = tiff etc. */
      tmp = strrchr(argv[2], '.');
      if(tmp)
         imlib_image_set_format(tmp + 1);
      /* save the image */
      imlib_save_image(argv[2]);
    }
}
