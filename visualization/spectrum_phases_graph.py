import matplotlib.pyplot as plt


class Save:
    def __init__(self, name):
        self.file_name = name
        self.time = []
        self.position = []

    def read_data(self):
        file = open(self.file_name, 'r')
        if not file:
            raise "Error of reading file"
        self.time = []
        self.position = []
        for line in file:
            if line == ("\n" or " \n"):
                continue
            list_of_input_string = line.split()
            for i in range(len(list_of_input_string)):
                if i == 0:
                    self.time.append(float(list_of_input_string[i]))
                elif i == 1:
                    self.position.append(float(list_of_input_string[i]))
        file.close()


def graphic(title, time, measure1, label1, output_file):
    plt.figure()
    plt.grid()
    plt.title(title)
    plt.plot(time, measure1, label=label1)
    plt.xlabel('')
    plt.ylabel('')
    plt.legend(loc='best')
    plt.savefig(output_file)
    plt.close()


if __name__ == '__main__':
    File = Save("./phase_out1.txt")
    File.read_data()
    graphic("Фазовый спектр", File.time, File.position, "Распределение фаз", "./phases_spectrum_graphic")
