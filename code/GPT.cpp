#include <complex>
#include <vector>
#include <cmath>

using namespace std;

// Compute the power spectrum of an image
vector<vector<double>> power_spectrum(const vector<vector<double>>& image) {
    int rows = image.size();
    int cols = image[0].size();

    // Compute the 2D FFT of the image
    vector<vector<complex<double>>> fft_image(rows, vector<complex<double>>(cols));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fft_image[i][j] = complex<double>(image[i][j], 0);
        }
    }
    fft2d(fft_image);

    // Compute the power spectrum of the FFT
    vector<vector<double>> power(rows, vector<double>(cols));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            power[i][j] = norm(fft_image[i][j]) / (rows * cols);
        }
    }

    return power;
}

// Apply a Wiener filter to an image
vector<vector<double>> wiener_filter(const vector<vector<double>>& image, 
                                     double noise_var, 
                                     int window_size) 
{
    int rows = image.size();
    int cols = image[0].size();

    // Compute the power spectrum of the image
    vector<vector<double>> power = power_spectrum(image);

    // Compute the Wiener filter
    vector<vector<double>> filter(rows, vector<double>(cols));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double weight = power[i][j] / (power[i][j] + noise_var);
            filter[i][j] = weight / (1 + weight);
        }
    }

    // Apply the Wiener filter to the image
    vector<vector<double>> filtered_image(rows, vector<double>(cols));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double sum = 0;
            double weight_sum = 0;
            for (int k = -window_size; k <= window_size; k++) {
                for (int l = -window_size; l <= window_size; l++) {
                    int x = i + k;
                    int y = j + l;
                    if (x >= 0 && x < rows && y >= 0 && y < cols) {
                        double weight = filter[k + window_size][l + window_size];
                        sum += weight * image[x][y];
                        weight_sum += weight;
                    }
                }
            }
            filtered_image[i][j] = sum / weight_sum;
        }
    }

    return filtered_image;
}

#define PI 3.14159265
int n;
 
int main(int argc, char **argv)
{
    cout << "Enter the size: ";
    cin >> n;
    double inputData[n][n];
    cout << "Enter the 2D elements ";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> inputData[i][j];
 
    double realOut[n][n];
    double imagOut[n][n];
    double amplitudeOut[n][n];
 
    int height = n;
    int width = n;
 
    // Two outer loops iterate on output data.
    for (int yWave = 0; yWave < height; yWave++)
    {
        for (int xWave = 0; xWave < width; xWave++)
        {
            // Two inner loops iterate on input data.
            for (int ySpace = 0; ySpace < height; ySpace++)
            {
                for (int xSpace = 0; xSpace < width; xSpace++)
                {
                    // Compute real, imag, and ampltude.
                    realOut[yWave][xWave] += (inputData[ySpace][xSpace] * cos(
                            2 * PI * ((1.0 * xWave * xSpace / width) + (1.0
                                    * yWave * ySpace / height)))) / sqrt(
                            width * height);
                    imagOut[yWave][xWave] -= (inputData[ySpace][xSpace] * sin(
                            2 * PI * ((1.0 * xWave * xSpace / width) + (1.0
                                    * yWave * ySpace / height)))) / sqrt(
                            width * height);
                    amplitudeOut[yWave][xWave] = sqrt(
                            realOut[yWave][xWave] * realOut[yWave][xWave]
                                    + imagOut[yWave][xWave]
                                            * imagOut[yWave][xWave]);
                }
                cout << realOut[yWave][xWave] << " + " << imagOut[yWave][xWave]
                        << " i (" << amplitudeOut[yWave][xWave] << ")\n";
            }
        }
    }
}