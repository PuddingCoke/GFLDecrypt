#include<iostream>
#include<string>
#include<filesystem>

#include<fstream>

using namespace std;

namespace fs = filesystem;

std::vector<char> Decrypt(const std::vector<char>& encrypt)
{
	int64_t length = encrypt.size() - 17ULL;
	std::vector<char> v24(16);
	std::vector<char> decrypt(length);
	for (int64_t i = 0; i < 16; i++)
	{
		v24[i] = encrypt[i * 5 + 1];
	}
	int64_t v20 = 0;
	int64_t v10 = -1;
	int64_t v11 = 0;
	do
	{
		int64_t v12 = v10 + 1;
		if (v10 + 1 > 80)
		{
			char v13 = v24[v10 - ((v10 + ((v10 - 16) >> 31 >> 28) - 16) & 0xFFFFFFF0) - 16];
			decrypt[v11] = (char)(encrypt[v10 + 1] ^ v13);
			v12 = v10 + 1;
			++v11;
		}
		else
		{
			if (v10 == 5 * (v10 / 5))
			{
				++v20;
			}
			else if (v10 != -1)
			{
				decrypt[v11++] = (char)(encrypt[v10 + 1] ^ v24[(v10 - v20) % 16]);
			}
		}
		v10 = v12;
	} while (v11 != length);
	return decrypt;
}

int main()
{

	for (const auto& entry : fs::directory_iterator("./"))
	{
		if (fs::status(entry.path()).type() == fs::file_type::directory || entry.path().extension().string() == ".exe")
		{
			continue;
		}

		cout << "Decrypt " << entry.path().string();

		ifstream stream(entry.path(), ios::binary);

		stream.seekg(0, ios::end);

		uint64_t bytesLength = stream.tellg();

		stream.seekg(0, ios::beg);

		std::vector<char> data(bytesLength);

		stream.read(&data[0], data.size());

		stream.close();

		data = Decrypt(data);

		ofstream outputStream;

		if (entry.path().extension().string() == ".bytes")
		{
			remove(entry.path().string().c_str());
			outputStream.open(entry.path().string().substr(0, entry.path().string().size() - 6), ios::binary);
		}
		else
		{
			outputStream.open(entry.path(), ios::trunc | ios::binary);
		}

		outputStream.write(&data[0], data.size());

		outputStream.close();

		cout << " complete!\n";

	}

	for (const auto& entry : fs::directory_iterator("./motions"))
	{

		cout << "Decrypt " << entry.path().string();

		ifstream stream(entry.path(), ios::binary);

		stream.seekg(0, ios::end);

		uint64_t bytesLength = stream.tellg();

		stream.seekg(0, ios::beg);

		std::vector<char> data(bytesLength);

		stream.read(&data[0], data.size());

		stream.close();

		data = Decrypt(data);

		ofstream outputStream;

		remove(entry.path().string().c_str());

		outputStream.open(entry.path().string().substr(0, entry.path().string().size() - 6), ios::binary);

		outputStream.write(&data[0], data.size());

		outputStream.close();

		cout << " complete!\n";

	}

	cin.get();

	return 0;
}