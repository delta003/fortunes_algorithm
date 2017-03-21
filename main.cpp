#include <iostream>
#include <istream>
#include <fstream>

#include "base/command_line_parser.h"
#include "base/logger.h"
#include "voronoi.h"
#include "point.h"
#include "edge.h"

using base::CmdParser;
using base::Logger;
using std::string;
using std::to_string;
using std::endl;

void read(std::istream& stream, Vertices* points) {
    int n;
    stream >> n;
    for (int i = 0; i < n; i++) {
        double x, y;
        stream >> x >> y;
        points->push_back(new Point(x, y));
    }
}
    
void readFromFile(string file_name, Vertices* points) {
    std::fstream stream(file_name, std::ios::in);
    read(stream, points);
}
    
void readFromConsole(Vertices* points) {
    read(std::cin, points);
}

void write(std::ostream& stream, Edges* edges) {
    stream << edges->size() << endl;
    for (Edges::iterator i = edges->begin(); i != edges->end(); i++) {
        Point* st = (*i)->start;
        Point* ed = (*i)->end;
        stream << st->x << " " << st->y << " " << ed->x << " " << ed->y << endl;
    }
}

void writeInFile(string file_name, Edges* edges) {
    std::fstream stream(file_name, std::ios::out);
    write(stream, edges);
}

void writeInConsole(Edges* edges) {
    write(std::cout, edges);
}
    
int main(int argc, char* argv[]) {
    if (!CmdParser::instance()->Parse(argc, argv)) {
        Logger::instance()->OutputError("Failed to parse command line arguments");
        return -1;
    }
    if (CmdParser::instance()->HaveArgument("log_file")) {
        string file_name = CmdParser::instance()->GetArgument("log_file");
        Logger::instance()->SetLogFile(file_name);
    }
    Vertices* points = new Vertices();
    if (CmdParser::instance()->HaveArgument("input_file")) {
        string file_name = CmdParser::instance()->GetArgument("input_file");
        readFromFile(file_name, points);
        Logger::instance()->OutputSuccess("Read " + to_string(points->size()) + " points from " + file_name);
    } else {
        readFromConsole(points);
    }
    Voronoi* fortunes_voronoi = new Voronoi();
    Logger::instance()->OutputInfo("Starts Fortune's algorithm...");
    Edges* solution_edges = fortunes_voronoi->GetEdges(points);
    Logger::instance()->OutputSuccess("Finished Fortune's algorithm");
    if (CmdParser::instance()->HaveArgument("output_file")) {
        string file_name = CmdParser::instance()->GetArgument("output_file");
        writeInFile(file_name, solution_edges);
        Logger::instance()->OutputSuccess("Solution written in " + file_name);
    } else {
        writeInConsole(solution_edges);
    }
    delete fortunes_voronoi;
    return 0;
}

