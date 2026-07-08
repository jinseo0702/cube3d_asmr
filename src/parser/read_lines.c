#include "../../include/cub3d.h"

#define READ_CHUNK 65536

void	free_lines(char **lines)
{
	int	i;

	i = 0;
	while (lines && lines[i])
		free(lines[i++]);
	free(lines);
}

static char	*read_whole_file(const char *path, size_t *size)
{
	char	*data;
	char	*bigger;
	ssize_t	got;
	int		fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (NULL);
	*size = 0;
	data = malloc(READ_CHUNK + 1);
	got = 1;
	while (data && got > 0)
	{
		got = read(fd, data + *size, READ_CHUNK);
		if (got > 0)
			*size += got;
		bigger = malloc(*size + READ_CHUNK + 1);
		if (bigger)
			ft_memcpy(bigger, data, *size);
		free(data);
		data = bigger;
	}
	close(fd);
	if (data)
		data[*size] = '\0';
	return (data);
}

static int	count_file_lines(const char *data, size_t size)
{
	size_t	i;
	int		count;

	count = 0;
	i = 0;
	while (i < size)
	{
		if (data[i] == '\n')
			count++;
		i++;
	}
	if (size > 0 && data[size - 1] != '\n')
		count++;
	return (count);
}

static char	*dup_range(const char *data, size_t start, size_t end)
{
	char	*line;

	line = malloc(end - start + 1);
	if (!line)
		return (NULL);
	ft_memcpy(line, data + start, end - start);
	line[end - start] = '\0';
	return (line);
}

/*
** Reads the whole file and splits it on '\n'. Empty lines are kept as ""
** entries so the parser can detect blank lines inside the map block.
*/
char	**read_all_lines(const char *path, int *count)
{
	char	**lines;
	char	*data;
	size_t	size;
	size_t	start;
	size_t	end;
	int		i;

	data = read_whole_file(path, &size);
	if (!data)
		return (NULL);
	*count = count_file_lines(data, size);
	lines = ft_calloc(*count + 1, sizeof(char *));
	start = 0;
	i = 0;
	while (lines && i < *count)
	{
		end = start;
		while (data[end] != '\0' && data[end] != '\n')
			end++;
		lines[i++] = dup_range(data, start, end);
		start = end + (data[end] == '\n');
	}
	free(data);
	return (lines);
}
