#include <iostream>
#include <random>
#include <cmath>
#include <complex>
#include <fstream>
#define PI 3.141592653589793

struct Input_Parameters {
    double phase;
    double frequency;
    double amplitude;

//    double noise;
//    double stand_deviation;
//    double exp_value;

};

struct Signal_Values {
    int num_of_measurements;
    double time_step;
    int num_of_measurements_per_second = 10000;
    int num_of_seconds = 2;
    double* position;
    double* time;
};

struct Fourie_Values {
    double* real_part;
    double* imag_part;
};

struct Amplitude_Spectrum {
    double* amplitudes;
    double* time;
};


void signal_uniform_generator(Input_Parameters& input_parameters, Signal_Values& generated_signal) {
    generated_signal.num_of_measurements = generated_signal.num_of_measurements_per_second * generated_signal.num_of_seconds;
    generated_signal.position = new double [generated_signal.num_of_measurements];
    generated_signal.time = new double [generated_signal.num_of_measurements];

    double time_counter = 0;
    generated_signal.time_step = 1.0 / generated_signal.num_of_measurements_per_second;
    for (int i = 0; i < generated_signal.num_of_measurements; ++i) {
        generated_signal.time[i] = time_counter;
        generated_signal.position[i] = input_parameters.amplitude * std::cos(input_parameters.frequency * 2 * PI * time_counter + input_parameters.phase);
        time_counter += generated_signal.time_step;
    }

    for (int i = 0; i < generated_signal.num_of_measurements; ++i) {
        generated_signal.time[i] = generated_signal.time[i] / generated_signal.num_of_seconds;
    }
}


void signal_non_uniform_generator(Input_Parameters& input_parameters, Signal_Values& generated_signal) {
    generated_signal.position = new double [generated_signal.num_of_measurements];
    generated_signal.time = new double [generated_signal.num_of_measurements];

    double time_counter = 0;
    int measurements_counter = 0;
    while (2 - time_counter <= 0.0001) {
        generated_signal.time[measurements_counter] = time_counter;
        generated_signal.position[measurements_counter] = input_parameters.amplitude * std::cos(input_parameters.frequency * time_counter + input_parameters.phase);
        time_counter += 0.0001 * (std::rand() % 10 + 1);
        ++measurements_counter;
    }
    generated_signal.num_of_measurements = measurements_counter;


    for (int i = 0; i < generated_signal.num_of_measurements; ++i) {
        generated_signal.time[i] = generated_signal.time[i] / generated_signal.num_of_seconds;
    }


}


Fourie_Values* NUDFT(Signal_Values& signal) {
    Fourie_Values* transformed_values = new Fourie_Values;
    transformed_values->imag_part = new double [signal.num_of_measurements];
    transformed_values->real_part = new double [signal.num_of_measurements];

    for (int k = 0; k < signal.num_of_measurements; ++k) {
        transformed_values->imag_part[k] = 0;
        transformed_values->real_part[k] = 0;
        for (int n = 0; n < signal.num_of_measurements; ++n) {
            transformed_values->imag_part[k] += signal.position[n] * std::sin(-2 * PI * signal.time[n] * ((double) k));
            transformed_values->real_part[k] += signal.position[n] * std::cos(-2 * PI * signal.time[n] * ((double) k));
        }
    }

    return transformed_values;
}


void write_transform_data(Fourie_Values* data, int num_of_values) {
    std::ofstream file;
    file.open("Fourie_Data.txt");

    for (int i = 0; i < num_of_values; ++i) {
        file << data->real_part[i] << "\t" << data->imag_part[i] << std::endl;
    }

    file.close();
}

double* find_amplitude_spectrum(Fourie_Values* transformed_values, int num_of_values) {
    double* amplitude_spectrum = new double [num_of_values];

    for (int i = 0; i < num_of_values; ++i) {
        amplitude_spectrum[i] = std::sqrt(transformed_values->real_part[i] * transformed_values->real_part[i] + transformed_values->imag_part[i] * transformed_values->imag_part[i]);
    }

    return amplitude_spectrum;
}

void write_amplitude_spectrum(double* spectrum, int num_of_values) {
    std::ofstream file;
    file.open("Amplitude_Spectrum.txt");

    for (int i = 0; i < num_of_values; ++i) {
        file << i << "\t" << spectrum[i] << std::endl;
    }

    file.close();
}

double restore_frequency(double* amplitudes, int num_of_measurements, int num_of_seconds) {
    double* frequencies = new double[num_of_measurements];
    for (int i = 0; i < num_of_measurements/2; ++i) {
        frequencies[i] = ((double) i) / num_of_seconds; // * num_of_seconds;
        frequencies[num_of_measurements - 1 - i] = - ((double) i) / num_of_seconds; // * num_of_seconds;
    }


    double max_amp = -1.0;
    int max_amp_ind = 0;
    for (int i = 0; i < num_of_measurements; ++i) {
        if (amplitudes[i] - max_amp > 0.00001) {
            max_amp = amplitudes[i];
            max_amp_ind = i;
        }
    }

    return frequencies[max_amp_ind];
}

int main() {
    Input_Parameters signal_parameters;
    std::cin >> signal_parameters.amplitude >> signal_parameters.frequency >> signal_parameters.phase;

    Signal_Values signal_1, signal_2;
    signal_uniform_generator(signal_parameters, signal_1);
    
    // signal_non_uniform_generator(signal_parameters, signal_2);

    Fourie_Values* transformed_signal = NUDFT(signal_1);
    // write_transform_data(transformed_signal, signal_2.num_of_measurements);

    double* spectrum = find_amplitude_spectrum(transformed_signal, signal_1.num_of_measurements);
    write_amplitude_spectrum(spectrum, signal_1.num_of_measurements);

    double freq = restore_frequency(spectrum, signal_1.num_of_measurements, signal_1.num_of_seconds);
    std::cout << freq << std::endl;
    
    double frequences_quotient = signal_parameters.frequency / freq;
    std::cout << "Frequences quotient: " << frequences_quotient << std::endl;


    return 0;
}

