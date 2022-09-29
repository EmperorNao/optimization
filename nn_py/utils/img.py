import cv2
import numpy as np
import pandas as pd
import os


def mnist_to_imgs(df):

    label_column = df.columns[0]
    pixel_columns = df.columns[1:]
    for index, row in df.iterrows():

        image = np.array(row[pixel_columns], dtype=np.uint8)
        image = image.reshape(28, 28)

        label = int(row[label_column])

        outdir = "/home/emperornao/projects/optimization/datasets/mnist"
        cv2.imwrite(os.path.join(outdir, '%d_%d.png' % (index, label)), image)
        print(float(index) / df.shape[0])


if __name__ == "__main__":

    train_data = pd.read_csv("/home/emperornao/projects/optimization/datasets/mnist_train.csv")
    mnist_to_imgs(train_data)