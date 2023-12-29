#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <random>

using namespace std;

bool verifiy(int argc, char *argv[])
{
    if (argc % 2 == 0)
    {
        return false;
    }

    for (int i = 1; i < argc; i++)
    {
        if (i % 2 == 0)
        {
            if (argv[i][0] == '-')
            {
                return false;
            }
        }
        else
        {
            if (argv[i][0] != '-')
            {
                return false;
            }
        }
    }

    return true;
}

double getRandomValue(double min, double max)
{
    std::random_device rd;

    std::mt19937 gen(rd());

    std::uniform_real_distribution<double> dis(min, max);

    return dis(gen);
}

int main(int argc, char *argv[])
{

    if (verifiy(argc, argv))
    {
        int num_sensors = 1;
        int sampling = 60;
        int interval = 24;
        auto currentTime = std::chrono::system_clock::now();

        const std::string fileName = "lux_sensor.csv";

        std::ofstream outputFile(fileName);

        if (!outputFile.is_open())
        {
            std::cerr << "Không thể mở file " << fileName << " để ghi." << std::endl;
            return 1;
        }

        outputFile << "id,time,value\n";

        // std::cout << std::put_time(localTime, "%y:%m:%d %H:%M:%S") << std::endl;

        for (int i = 1; i < argc; i += 2)
        {
            if (argv[i][1] == 'n')
            {
                num_sensors = std::stoi(argv[i + 1]);
            }
            else
            {
                if (argv[i][2] == 't')
                {
                    sampling = std::stoi(argv[i + 1]);
                }
                else
                {
                    interval = std::stoi(argv[i + 1]);
                }
            }
        }

        int cnt = interval * 60 * 60 / sampling;

        for (int i = 0; i < cnt; i++)
        {
            auto newTime = currentTime + std::chrono::seconds(i * sampling - interval * 60 * 60);

            std::time_t newTimeT = std::chrono::system_clock::to_time_t(newTime);

            std::tm *localTime = std::localtime(&newTimeT);

            for (int j = 0; j < num_sensors; j++)
            {
                outputFile << j + 1 << "," << std::put_time(localTime, "%y:%m:%d %H:%M:%S") << ",";
                outputFile << getRandomValue(0.1, 40000) << std::fixed << std::setprecision(2) << '\n';
            }
        }

        cout << "Num: " << num_sensors << " Sampling: " << sampling << " Interval: " << interval << '\n';
        outputFile.close();
    }
    else
    {
        cout << "ERROR\n";
    }

    return 0;
}
