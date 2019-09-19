from absl import flags, app
import copy

FLAGS = flags.FLAGS

flags.DEFINE_string("path", "./config.txt", "Path to configuration file.")


def read_config(path):
    with open(path, "r") as file:
        config_args = dict([line.split("=") for line in file if not line.strip().startswith("#")])

    arguments = ["dt", "dx", "dy", "k", "c", "density"])
    dt, dx, dy, k, c, density = map(lambda x: float(config_args[x]), arguments)

    with open(config_args["path_to_matrix"], "r") as file:
        matrix = [[float(num) for num in line.split()] for line in file]

    return matrix, dt, dx, dy, k / (c * density)


def compute_new_temperature(path):
    matrix, dt, dx, dy, alpha = read_config(path)

    new_matrix = copy.deepcopy(matrix)

    while input("Press Enter to print new matrix: ") == "":
        for i in range(1, len(matrix) - 1):
            for j in range(1, len(matrix[0]) - 1):
                new_matrix[i][j] = matrix[i][j] + dt * alpha * (
                        (matrix[i - 1][j] - 2 * matrix[i][j] + matrix[i + 1][j]) / (dx ** 2) + (
                          matrix[i][j - 1] - 2 * matrix[i][j] + matrix[i][j + 1]) / (dy ** 2))

        matrix = copy.deepcopy(new_matrix)

        print("\n".join([" ".join(map(lambda x: f"{x:6.2f}", line)) for line in matrix]))


def main(argv):
    del argv  # Unused.
    compute_new_temperature(FLAGS.path)


if __name__ == "__main__":
    app.run(main)
