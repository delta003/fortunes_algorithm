"""
    Interactive demo for Voronoi diagram
"""

import os
import argparse
import random
import matplotlib.pyplot as plt


points = []
window_size = 1000.0
log_file = None


def onclick(event):
    plt.close()
    points.append((event.xdata, event.ydata))
    print('Point (' + str(event.xdata) + ', ' + str(event.ydata) + ') added')
    reload()


def visualize():
    file_points = 'points_tmp.txt'
    file_diagram = 'diagram_tmp.txt'
    xs = []
    ys = []
    with open(file_points, 'r') as file:
        lines = file.readlines()
        n = int(lines[0])
        for i in range(1, n + 1):
            parts = lines[i].split(' ')
            x = float(parts[0])
            y = float(parts[1])
            xs.append(x)
            ys.append(y)
            file.close()
    diagram_lines = []
    with open(file_diagram, 'r') as file:
        lines = file.readlines()
        n = int(lines[0])
        for i in range(1, n + 1):
            parts = lines[i].split(' ')
            x1 = float(parts[0])
            y1 = float(parts[1])
            x2 = float(parts[2])
            y2 = float(parts[3])
            diagram_lines.append((x1, y1, x2, y2))
    plt.figure(0)
    plt.plot(xs, ys, 'o', color = 'black')
    for i in range(0, len(diagram_lines)):
        x = [diagram_lines[i][0], diagram_lines[i][2]]
        y = [diagram_lines[i][1], diagram_lines[i][3]]
        plt.plot(x, y, color = 'red')
    plt.xlim((0.0, window_size))
    plt.ylim((0.0, window_size))
    plt.figure(0).canvas.mpl_connect('button_release_event', onclick)
    plt.show()


def run_algorithm():
    print('Running algorithm...')
    lines = [str(len(points)) + '\n']
    for i in range(0, len(points)):
        lines.append(str(points[i][0]) + ' ' + str(points[i][1]) + '\n')
    with open('points_tmp.txt', 'w') as file:
        file.writelines(lines)
        file.close()
    cmd = './main.out --input_file points_tmp.txt --output_file diagram_tmp.txt'
    if log_file is not None:
        cmd += ' --log_file {0}'.format(log_file)
    os.system(cmd)
    print('Done')


def reload():
    run_algorithm()
    visualize()


def main():
    parser = argparse.ArgumentParser(description = 'Process arguments')
    parser.add_argument('--start_points', type = int, default = 10,
                        help = 'initial number of points')
    parser.add_argument('--window_size', type = float, default = 1000.0,
                        help = 'plotting window size')
    parser.add_argument('--log_file', type = str, default = None,
                        help = 'logs from running an algorithm')
    args = parser.parse_args()
    global log_file
    log_file = args.log_file
    window_size = args.window_size
    for i in range(0, args.start_points):
        x = random.random() * window_size
        y = random.random() * window_size
        points.append((x, y))
    reload()


if __name__ == '__main__':
    main()