from absl import flags, app

FLAGS = flags.FLAGS

flags.DEFINE_string("path", "./matrix.txt", "Path to file for matrix.")
flags.DEFINE_integer("height", 10, "Height of matrix")
flags.DEFINE_integer("width", 10, "Width of matrix")
flags.DEFINE_integer("top", 99, "Temperature at top border")
flags.DEFINE_integer("bottom", 60, "Temperature at bottom border")
flags.DEFINE_integer("left", 50, "Temperature at left border")
flags.DEFINE_integer("right", 10, "Temperature at right border")
flags.DEFINE_integer("center", 20, "Temperature at right border")


def create_matrix(path, height, width, top, bottom, left, right, center):
    top, bottom, left, right, center = map(str, [top, bottom, left, right, center])
    with open(path, "w") as file:
        file.write(" ".join([top] * width) + "\n")
        for i in range(height - 2):
            file.write(" ".join([left] + [center] * (width - 2) + [right]) + "\n")
        file.write(" ".join([bottom] * width) + "\n")


def main(argv):
    del argv  # Unused.
    create_matrix(FLAGS.path, FLAGS.height, FLAGS.width, FLAGS.top, FLAGS.bottom, FLAGS.left, FLAGS.right, FLAGS.center)


if __name__ == "__main__":
    app.run(main)
