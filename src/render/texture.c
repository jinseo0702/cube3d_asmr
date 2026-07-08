#include "../../include/cub3d.h"

typedef struct s_xpm
{
	char		**strs;
	int			count;
	int			ncolors;
	int			cpp;
	char		*keys;
	uint32_t	*colors;
}	t_xpm;

/*
** Minimal XPM3 reader: enough for the plain "<chars> c #RRGGBB" palettes
** produced by GIMP and our texture generator. Keeps the renderer free of
** any image library dependency on both backends.
*/
static char	*extract_quoted(const char *line)
{
	const char	*start;
	const char	*end;

	start = ft_strchr(line, '"');
	if (!start)
		return (NULL);
	end = ft_strrchr(start + 1, '"');
	if (!end || end <= start)
		return (NULL);
	return (ft_strndup(start + 1, end - start - 1));
}

static int	collect_strings(const char *path, t_xpm *xpm)
{
	char	**lines;
	char	*quoted;
	int		total;
	int		i;

	lines = read_all_lines(path, &total);
	if (!lines)
		return (-1);
	xpm->strs = ft_calloc(total + 1, sizeof(char *));
	xpm->count = 0;
	i = 0;
	while (xpm->strs && i < total)
	{
		quoted = extract_quoted(lines[i]);
		if (quoted)
			xpm->strs[xpm->count++] = quoted;
		i++;
	}
	free_lines(lines);
	if (!xpm->strs || xpm->count == 0)
		return (-1);
	return (0);
}

static int	parse_header(const char *s, t_tex *tex, t_xpm *xpm)
{
	int	values[4];
	int	i;

	i = 0;
	while (i < 4)
	{
		while (*s == ' ' || *s == '\t')
			s++;
		if (!ft_isdigit(*s))
			return (-1);
		values[i++] = ft_atoi(s);
		while (ft_isdigit(*s))
			s++;
	}
	tex->width = values[0];
	tex->height = values[1];
	xpm->ncolors = values[2];
	xpm->cpp = values[3];
	if (tex->width <= 0 || tex->height <= 0 || xpm->ncolors <= 0
		|| xpm->cpp <= 0 || xpm->cpp > 4)
		return (-1);
	return (0);
}

static int	hex_digit(char c)
{
	if (ft_isdigit(c))
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (-1);
}

static int	parse_color_value(const char *s, uint32_t *color)
{
	uint32_t	value;
	int			digit;
	int			i;

	if (ft_strncmp(s, "None", 4) == 0)
	{
		*color = 0xFF000000u;
		return (0);
	}
	if (*s != '#')
		return (-1);
	value = 0;
	i = 1;
	while (i <= 6)
	{
		digit = hex_digit(s[i]);
		if (digit < 0)
			return (-1);
		value = value * 16 + digit;
		i++;
	}
	*color = 0xFF000000u | value;
	return (0);
}

static int	parse_palette(t_xpm *xpm)
{
	const char	*s;
	int			i;

	xpm->keys = ft_calloc(xpm->ncolors, xpm->cpp);
	xpm->colors = ft_calloc(xpm->ncolors, sizeof(uint32_t));
	if (!xpm->keys || !xpm->colors || xpm->count < 1 + xpm->ncolors)
		return (-1);
	i = 0;
	while (i < xpm->ncolors)
	{
		s = xpm->strs[1 + i];
		if ((int)ft_strlen(s) < xpm->cpp)
			return (-1);
		ft_memcpy(xpm->keys + i * xpm->cpp, s, xpm->cpp);
		s += xpm->cpp;
		while (*s && !(*s == 'c' && (s[1] == ' ' || s[1] == '\t')))
			s++;
		if (*s == '\0')
			return (-1);
		s += 2;
		while (*s == ' ' || *s == '\t')
			s++;
		if (parse_color_value(s, &xpm->colors[i]) < 0)
			return (-1);
		i++;
	}
	return (0);
}

static int	lookup_key(t_xpm *xpm, const char *s)
{
	int	i;

	i = 0;
	while (i < xpm->ncolors)
	{
		if (ft_memcmp(xpm->keys + i * xpm->cpp, s, xpm->cpp) == 0)
			return (i);
		i++;
	}
	return (-1);
}

static int	parse_pixels(t_xpm *xpm, t_tex *tex)
{
	const char	*row;
	int			x;
	int			y;
	int			key;

	tex->pixels = ft_calloc(tex->width * tex->height, sizeof(uint32_t));
	if (!tex->pixels || xpm->count < 1 + xpm->ncolors + tex->height)
		return (-1);
	y = 0;
	while (y < tex->height)
	{
		row = xpm->strs[1 + xpm->ncolors + y];
		if ((int)ft_strlen(row) < tex->width * xpm->cpp)
			return (-1);
		x = 0;
		while (x < tex->width)
		{
			key = lookup_key(xpm, row + x * xpm->cpp);
			if (key < 0)
				return (-1);
			tex->pixels[y * tex->width + x] = xpm->colors[key];
			x++;
		}
		y++;
	}
	return (0);
}

static void	xpm_cleanup(t_xpm *xpm)
{
	free_lines(xpm->strs);
	free(xpm->keys);
	free(xpm->colors);
}

int	tex_load_xpm(t_tex *tex, const char *path)
{
	t_xpm	xpm;
	int		ret;

	ft_memset(&xpm, 0, sizeof(t_xpm));
	ft_memset(tex, 0, sizeof(t_tex));
	ret = collect_strings(path, &xpm);
	if (ret == 0)
		ret = parse_header(xpm.strs[0], tex, &xpm);
	if (ret == 0)
		ret = parse_palette(&xpm);
	if (ret == 0)
		ret = parse_pixels(&xpm, tex);
	xpm_cleanup(&xpm);
	if (ret < 0)
	{
		ft_putstr_fd("Error\ncannot load texture: ", 2);
		ft_putstr_fd((char *)path, 2);
		ft_putstr_fd("\n", 2);
	}
	return (ret);
}
