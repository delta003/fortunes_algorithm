"""
    Script for visualizing points and generated Voronoi diagram
  
    Takes two command line arguments
        - points - description of points
            (number of points followed by that many rows with two real numbers (x, y))
        - diagram - description of Voronoi diagram
            (number of edges followed by that many rows with four real numbers (st.x, st.y, ed.x, ed.y))
"""

import argparse
import matplotlib.pyplot as plt


def plot_end(points, diagram):
    xmin = points[0][0]
    xmax = points[0][0]
    ymin = points[1][0]
    ymax = points[1][0]
    for i in range(0, len(points[0])):
        xmin = min(xmin, points[0][i])
        xmax = max(xmax, points[0][i])
        ymin = min(ymin, points[1][i])
        ymax = max(ymax, points[1][i])
    plt.plot(points[0], points[1], 'o', color = 'black')
    for i in range(0, len(diagram)):
        x = [diagram[i][0], diagram[i][2]]
        y = [diagram[i][1], diagram[i][3]]
        plt.plot(x, y, color = 'red')
    plt.xlim((xmin - 50.0, xmax + 50.0))
    plt.ylim((ymin - 50.0, ymax + 50.0))
    plt.show()


def main():
    parser = argparse.ArgumentParser(description = 'Process arguments')
    parser.add_argument('--points', type = str, default = 'input.txt',
                        help = 'input points, see description (default: input.txt)')
    parser.add_argument('--diagram', type = str, default = 'output.txt',
                        help = 'Voronoi diagram, see description (default: output.txt)')
    args = parser.parse_args()
    points = args.points
    diagram = args.diagram
    xs = []
    ys = []
    with open(points, 'r') as file:
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
    with open(diagram, 'r') as file:
        lines = file.readlines()
        n = int(lines[0])
        for i in range(1, n + 1):
            parts = lines[i].split(' ')
            x1 = float(parts[0])
            y1 = float(parts[1])
            x2 = float(parts[2])
            y2 = float(parts[3])
            diagram_lines.append((x1, y1, x2, y2))
    plot_end((xs, ys), diagram_lines)


if __name__ == '__main__':
    main()