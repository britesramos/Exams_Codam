#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>

#define MAX_SIZE 1000

void print_error() {
	write(1, "Map Error\n", 10);
}

void dfs(char **grid, int **visited, int i, int j, int rows, int cols, int *count) {
	if (i < 0 || j < 0 || i >= rows || j >= cols) return;
	if (grid[i][j] != 'X' || visited[i][j]) return;

	visited[i][j] = 1;
	(*count)++;

	dfs(grid, visited, i + 1, j, rows, cols, count);
	dfs(grid, visited, i - 1, j, rows, cols, count);
	dfs(grid, visited, i, j + 1, rows, cols, count);
	dfs(grid, visited, i, j - 1, rows, cols, count);
}

int main(int argc, char **argv) {
	if (argc != 2) {
		print_error();
		return 1;
	}

	int fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		print_error();
		return 1;
	}

	struct stat st;
	if (fstat(fd, &st) < 0 || st.st_size == 0) {
		close(fd);
		print_error();
		return 1;
	}

	char *buf = malloc(st.st_size);
	if (!buf) {
		close(fd);
		print_error();
		return 1;
	}

	ssize_t bytes_read = read(fd, buf, st.st_size);
	close(fd);
	if (bytes_read != st.st_size) {
		free(buf);
		print_error();
		return 1;
	}

	int rows = 0, cols = -1, len = 0;
	for (ssize_t i = 0; i < bytes_read; i++) {
		if (buf[i] == '\n') {
			if (cols == -1)
				cols = len;
			else if (len != cols) {
				free(buf);
				print_error();
				return 1;
			}
			rows++;
			len = 0;
		} else if (buf[i] == '.' || buf[i] == 'X') {
			len++;
		} else {
			free(buf);
			print_error();
			return 1;
		}
	}

	if (rows == 0 || cols == 0 || rows > MAX_SIZE || cols > MAX_SIZE) {
		free(buf);
		print_error();
		return 1;
	}

	char **grid = malloc(rows * sizeof(char *));
	int **visited = malloc(rows * sizeof(int *));
	if (!grid || !visited) {
		free(buf);
		free(grid);
		free(visited);
		print_error();
		return 1;
	}

	for (int i = 0; i < rows; i++) {
		grid[i] = malloc(cols);
		visited[i] = malloc(cols * sizeof(int));
		if (!grid[i] || !visited[i]) {
			for (int j = 0; j <= i; j++) {
				free(grid[j]);
				free(visited[j]);
			}
			free(grid);
			free(visited);
			free(buf);
			print_error();
			return 1;
		}
	}

	ssize_t pos = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			grid[i][j] = buf[pos++];
			visited[i][j] = 0;
		}
		pos++; // skip newline
	}
	free(buf);

	int max_island = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (grid[i][j] == 'X' && !visited[i][j]) {
				int count = 0;
				dfs(grid, visited, i, j, rows, cols, &count);
				if (count > max_island)
					max_island = count;
			}
		}
	}

	char out[12];
	int n = max_island, i = 11;
	out[i--] = '\n';
	if (n == 0) {
		out[i--] = '0';
	} else {
		while (n > 0) {
			out[i--] = '0' + (n % 10);
			n /= 10;
		}
	}
	write(1, &out[i + 1], 11 - i);

	for (int i = 0; i < rows; i++) {
		free(grid[i]);
		free(visited[i]);
	}
	free(grid);
	free(visited);
	return 0;
}
