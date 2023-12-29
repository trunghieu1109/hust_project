#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <random>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int location_min[] = {20, 50, 100, 100, 150, 200, 250, 300, 500, 750, 1500, 2000, 5000, 10000};
int location_max[] = {50, 100, 200, 150, 250, 400, 350, 500, 700, 850, 2000, 5000, 10000, 20000};

std::tm convertStringToTm(const std::string &timeString)
{
    std::tm tm = {};
    std::istringstream ss(timeString);
    ss >> std::get_time(&tm, "%Y:%m:%d %H:%M:%S");

    if (ss.fail())
    {
        // Xử lý lỗi khi đọc thời gian từ xâu
        throw std::runtime_error("Invalid time format");
    }

    return tm;
}

// Hàm tính số giờ giữa hai std::tm
int calculateHourDifference(const std::tm &start, const std::tm &end)
{
    // Tạo bản sao của tm
    std::tm startCopy = start;
    std::tm endCopy = end;

    // Chuyển tm thành std::time_t
    std::time_t startTime_t = std::mktime(&startCopy);
    std::time_t endTime_t = std::mktime(&endCopy);

    if (startTime_t == -1 || endTime_t == -1)
    {
        // Xử lý lỗi khi chuyển đổi std::tm sang std::time_t
        throw std::runtime_error("Failed to convert std::tm to std::time_t");
    }

    std::chrono::system_clock::time_point startTime = std::chrono::system_clock::from_time_t(startTime_t);
    std::chrono::system_clock::time_point endTime = std::chrono::system_clock::from_time_t(endTime_t);

    auto duration = std::chrono::duration_cast<std::chrono::hours>(endTime - startTime);

    return duration.count();
}

bool verifiy(int argc, char *argv[])
{
    if (argc < 3)
    {
        return false;
    }

    return true;
}

std::vector<std::string> splitString(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;

    while ((end = s.find(delimiter, start)) != std::string::npos)
    {
        tokens.push_back(s.substr(start, end - start));
        start = end + 1;
    }

    tokens.push_back(s.substr(start));

    return tokens;
}

bool isOutlier(double value)
{
    if (value < 1.0)
    {
        return true;
    }

    if (value > 30000.0)
    {
        return true;
    }

    return false;
}

std::vector<std::vector<std::string>> getOutliers(std::vector<std::vector<std::string>> data)
{

    std::vector<std::vector<std::string>> split_outliers;

    std::vector<std::string> outliers;
    std::vector<std::string> not_outliers;

    for (int i = 1; i < data.size(); i++)
    {
        // cout << stod(data[i][2]);

        double value = std::stof(data[i][2]);

        if (isOutlier(value))
        {
            outliers.push_back(data[i][0] + "," + data[i][1] + "," + data[i][2]);
        }
        else
        {
            not_outliers.push_back(std::to_string(i));
        }
    }

    split_outliers.push_back(outliers);
    split_outliers.push_back(not_outliers);

    return split_outliers;
}

string getCondition(double value, int location_id)
{
    if (value < location_min[location_id])
    {
        return "dark";
    }

    if (value > location_max[location_id])
    {
        return "bright";
    }

    return "good";
}

int main(int argc, char *argv[])
{

    if (verifiy(argc, argv))
    {
        ifstream inputData(argv[1]);

        std::vector<std::vector<std::string>> data;
        std::vector<std::vector<std::string>> location;

        if (!inputData.is_open())
        {
            std::cerr << "Error opening data file." << std::endl;
            return 1;
        }
        else
        {
            std::string line;
            while (std::getline(inputData, line))
            {
                std::vector<std::string> tokens = splitString(line, ',');

                data.push_back(tokens);
            }
        }

        ifstream inputLocation(argv[2]);

        if (!inputLocation.is_open())
        {
            std::cerr << "Error opening location file." << std::endl;
            return 1;
        }
        else
        {
            std::string line;
            while (std::getline(inputLocation, line))
            {
                std::vector<std::string> tokens = splitString(line, ',');

                location.push_back(tokens);
            }
        }

        // Task 2.1

        vector<vector<string>> split_outliers = getOutliers(data);
        vector<string> outliers = split_outliers[0];
        ofstream ofsOutliers("lux_outlier.csv");

        if (!ofsOutliers.is_open())
        {
            std::cerr << "Error opening location file." << std::endl;
        }
        else
        {
            ofsOutliers << "number of outliers: " << outliers.size() << '\n';
            ofsOutliers << "id,time,value" << '\n';
            for (int i = 0; i < outliers.size(); i++)
            {
                ofsOutliers << outliers[i] << '\n';
            }
            ofsOutliers.close();
        }

        // Task 2.2

        vector<string> not_outliers = split_outliers[1];
        ofstream ofsCondition("lux_condition.csv");
        if (!ofsCondition.is_open())
        {
            std::cerr << "Error opening location file." << std::endl;
        }
        else
        {
            ofsCondition << "id,time,location,value,condition" << '\n';

            for (int i = 0; i < not_outliers.size(); i++)
            {
                int id = stoi(not_outliers[i]);

                if (location[stoi(data[id][0])].size() > 1)
                {
                    int location_id = stoi(location[stoi(data[id][0])][1]);

                    ofsCondition << data[id][0] + "," + data[id][1] + "," << location[stoi(data[id][0])][1] << "," + data[id][2] << "," << getCondition(stod(data[id][2]), location_id) << '\n';
                }
                else
                {
                    ofsCondition << data[id][0] + "," + data[id][1] + "," << location[stoi(data[id][0])][1] << "," + data[id][2] << ","
                                 << "NA" << '\n';
                }
            }

            ofsCondition.close();
        }

        // Task 2.3

        int num_sensors = location.size() + 2;
        int max[num_sensors];
        double max_value[num_sensors];
        int min[num_sensors];
        double min_value[num_sensors];
        double total[num_sensors];
        int count_sensors[num_sensors];

        // set default values
        memset(total, 0, num_sensors * sizeof(double));
        memset(max, 0, num_sensors * sizeof(int));
        memset(max_value, 0, num_sensors * sizeof(double));
        memset(min, 0, num_sensors * sizeof(int));
        memset(count_sensors, 0, num_sensors * sizeof(int));

        for (int i = 0; i < num_sensors; i++)
        {
            min_value[i] = 100000;
        }

        for (int i = 0; i < not_outliers.size(); i++)
        {
            int id = stoi(not_outliers[i]);
            int sensor_id = stoi(data[id][0]);
            double value = stod(data[id][2]);
            if (value > max_value[sensor_id])
            {
                max_value[sensor_id] = value;
                max[sensor_id] = id;
            }

            if (value < min_value[sensor_id])
            {
                min_value[sensor_id] = value;
                min[sensor_id] = id;
            }

            total[sensor_id] = total[sensor_id] + value;
            count_sensors[sensor_id]++;
        }

        // calculate measured time

        std::string startTimeString = data[1][1];
        std::string endTimeString = data[data.size() - 1][1];
        int hourDifference = 0;

        try
        {
            std::tm startTm = convertStringToTm(startTimeString);
            std::tm endTm = convertStringToTm(endTimeString);
            hourDifference = calculateHourDifference(startTm, endTm) + 1;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error" << e.what() << std::endl;
        }

        ofstream ofsSummary("lux_summary.csv");

        if (!ofsSummary.is_open())
        {
            std::cerr << "Error opening location file." << std::endl;
        }
        else
        {

            ofsSummary << "id, parameter,time,value" << '\n';

            for (int i = 0; i < num_sensors - 3; i++)
            {
                ofsSummary << to_string(i + 1) + ",max," + data[max[i + 1]][1] + ",";
                ofsSummary << max_value[i + 1] << std::fixed << std::setprecision(2) << '\n';

                ofsSummary << to_string(i + 1) + ",min," + data[min[i + 1]][1] + ",";
                ofsSummary << min_value[i + 1] << std::fixed << std::setprecision(2) << '\n';

                ofsSummary << to_string(i + 1) + ",mean," + to_string(hourDifference) + ":00:00" + ",";
                ofsSummary << total[i + 1] / count_sensors[i + 1] << std::fixed << std::setprecision(2) << '\n';
            }

            ofsSummary.close();
        }
    }
    else
    {
        cout << "ERROR\n";
    }

    return 0;
}
