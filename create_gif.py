from absl import flags, app
import imageio
from os import listdir
from os.path import join

FLAGS = flags.FLAGS

flags.DEFINE_string("images_path", "images/", "Path to folder file.")
flags.DEFINE_string("output_path", "heatmap.gif", "Path to configuration file.")
flags.DEFINE_float("time", 0.1, "Time between frames.")


def create_gif(images_path, output_path, time):
    filenames = [join(images_path, f) for f in listdir(images_path) if f.endswith(".bmp")]
    with imageio.get_writer(output_path, mode='I', duration=time) as writer:
        for filename in sorted(filenames, key=lambda x: int(x.split("-")[1].split(".")[0])):
            image = imageio.imread(filename)
            writer.append_data(image)


def main(argv):
    del argv  # Unused.
    create_gif(FLAGS.images_path, FLAGS.output_path, FLAGS.time)


if __name__ == "__main__":
    app.run(main)
