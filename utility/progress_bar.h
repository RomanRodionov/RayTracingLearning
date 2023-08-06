#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

class ProgressBar
{
    int total, done;
    double progress;
    public:
        ProgressBar(int t) : total(t), done(0), progress(0) {}
        void print(std::string s)
        {
            std::cout << s << ": 000%" << std::flush;
        }
        void increase(int p)
        {
            done += p;
        }
        void update()
        {
            double progress = (int)((double) done / total * 100.0);
            std::ostringstream ss;
            ss << std::setw(3) << std::setfill('0') << progress;
            std::string progress_str = ss.str();
            std::cout << "\b\b\b\b" << progress_str << "%" << std::flush;
        }
};

#endif