#include "../../include/cub3d.h"

int	fb_init(t_fb *fb, int width, int height)
{
	fb->width = width;
	fb->height = height;
	fb->pixels = ft_calloc(width * height, sizeof(uint32_t));
	if (!fb->pixels)
		return (-1);
	return (0);
}

void	fb_clear(t_fb *fb, uint32_t color)
{
	int	i;

	i = 0;
	while (i < fb->width * fb->height)
		fb->pixels[i++] = color;
}

void	fb_put_pixel(t_fb *fb, int x, int y, uint32_t color)
{
	if (x < 0 || x >= fb->width || y < 0 || y >= fb->height)
		return ;
	fb->pixels[y * fb->width + x] = color;
}

/*
** Dumps the framebuffer as a binary PPM (P6) image. Used by the
** --screenshot flag to render without opening a window.
*/
int	fb_write_ppm(t_fb *fb, const char *path)
{
	FILE		*fp;
	uint32_t	px;
	uint8_t		rgb[3];
	int			i;

	fp = fopen(path, "wb");
	if (!fp)
		return (error_msg("cannot open screenshot output file"));
	fprintf(fp, "P6\n%d %d\n255\n", fb->width, fb->height);
	i = 0;
	while (i < fb->width * fb->height)
	{
		px = fb->pixels[i++];
		rgb[0] = (px >> 16) & 0xFF;
		rgb[1] = (px >> 8) & 0xFF;
		rgb[2] = px & 0xFF;
		fwrite(rgb, 1, 3, fp);
	}
	fclose(fp);
	return (0);
}
