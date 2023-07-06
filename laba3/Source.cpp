#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>

int32_t calculateNumBins(int32_t numNumbers) {
    if (!numNumbers) {
        std::cerr << "Enter numbers: ";
        std::cin >> numNumbers;
    }
    
    //��������� ���������� ��������

    double K = std::sqrt(numNumbers);
    int32_t numBins = (K > 25) ? (1 + static_cast<int32_t>(std::log2(numNumbers))) : static_cast<int32_t>(K);

    std::cerr << "The number of columns is selected according to the "
        << ((K > 25) ? "Sturges formula: " : "empirical formula: ")
        << numBins << '\n';

    return numBins;
}

std::vector<int32_t> countBins(const std::vector<int32_t>& numbers, int32_t numBins, int32_t& minVal, int32_t& maxVal) {
    minVal = maxVal = numbers[0];//kol-vo bin

    for (auto num : numbers) {
        minVal = std::min(minVal, num);
        maxVal = std::max(maxVal, num);
    }

    std::vector<int32_t> counters(numBins, 0);
    auto binSize = static_cast<double>(maxVal - minVal + 1) / numBins;

    for (auto num : numbers) {
        auto binIndex = static_cast<int32_t>(std::floor((num - minVal) / binSize));
        counters[binIndex]++;
    }

    return counters;
}
//������ � ������������ ������� � ������ ���� �����������
void createSVG(const std::vector<int32_t>& binCounters, int32_t minVal, int32_t maxVal, const std::string& filename) {
    std::ofstream file(filename);

    auto maxCount = *std::max_element(binCounters.begin(), binCounters.end());
    auto numBins = binCounters.size();
    auto width = 30 * numBins;
    auto height = 300;

    file << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << width << "\" height=\"" << height + 50 << "\">\n"; // ������ ��������� SVG-��������� � ����� � ��������� ������ � ������.

    for (auto i = 0; i < numBins; ++i) {
        auto barHeight = static_cast<int32_t>((static_cast<double>(binCounters[i]) / maxCount) * height); //������ ������� 
        auto colorVal = 10 - static_cast<int32_t>((binCounters[i] * 9.0) / maxCount); //�������� ����� . �� ���������� ����� � ������. 
        //��� ������ �����, ��� ����� ������� ����.

        std::stringstream ss;
        ss << std::hex << colorVal;
//��������� ������ 1 ��� �������������� 2 ����������������� ��������.

        std::string color = "#" + ss.str() + ss.str() + ss.str();

        auto x = i * 30;
        auto y = height - barHeight;
       //v���������� ���������� ��� ������� ������� 

        file << "<rect x=\"" << x << "\" y=\"" << y << "\" width=\"25\" height=\"" << barHeight << "\" style=\"fill:" << color << ";\"/>\n";
        file << "<text x=\"" << x << "\" y=\"" << height + 15 << "\" font-size=\"18\">" << binCounters[i] << "</text>\n";
    }

    file << "</svg>";
    file.close();
}

int main() {
    int32_t numNumbers;
    std::cerr << "Enter amount of numbers: ";
    std::cin >> numNumbers;

    std::vector<int32_t> numbers(numNumbers);
    std::cerr << "Enter numbers: ";
    for (auto& i : numbers)
        std::cin >> i;

    int32_t numBins = calculateNumBins(numNumbers); //kol-vo ��������

    int32_t minVal, maxVal;
    std::vector<int32_t> binCounters = countBins(numbers, numBins, minVal, maxVal);

    createSVG(binCounters, minVal, maxVal, "histogram.svg"); //������ �����������
    return 0;
}