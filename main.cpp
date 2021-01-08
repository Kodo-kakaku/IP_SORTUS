//#include <cassert>
//#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>


template <typename... Params>
std::vector<std::vector<unsigned>> filter_ip_pool(std::vector<std::vector<unsigned>>& ip_pool, Params... args) {
    std::vector<std::vector<unsigned>> ip_address;
    std::vector<unsigned> inp = { args... };
    
    for (const auto& ip : ip_pool) {
        auto add(true);
        for (size_t i = 0; i < inp.size(); ++i) {
            if (inp.at(i) != ip.at(i)) {
                add = false;
            }
        }
        if (add) {
            ip_address.push_back(ip);
        }
    }
    return ip_address;
}

std::vector<std::string> split(const std::string& str, char d) {
	std::vector<std::string> r;

	std::string::size_type start = 0;
	std::string::size_type stop = str.find_first_of(d);
	while (stop != std::string::npos) {
		r.push_back(str.substr(start, stop - start));

		start = stop + 1;
		stop = str.find_first_of(d, start);
	}

	r.push_back(str.substr(start));

	return r;
}

std::vector<unsigned> convert(const std::vector<std::string>& ip_str) {

	std::vector<unsigned> ip_address;

	for (auto&& octet : ip_str) {
        octet.empty() ? ip_address.push_back(-1) : ip_address.push_back(std::stoi(octet) + '\0');
    }

	return ip_address;
}

std::vector<std::vector<unsigned>> filter_ip_pool_any(const std::vector<std::vector<unsigned>>& ip_pool, const unsigned arg) {

	std::vector<std::vector<unsigned>> ip_address;

    std::copy_if(ip_pool.begin(), ip_pool.end(), back_inserter(ip_address),
        [arg](const auto& i)
        { return std::any_of(i.begin(), i.end(), [arg](const auto& i) {return i == arg; }); });
    
    return ip_address;
}

void ip_print(const std::vector<std::vector<unsigned>>& ip_pool)
{
    for (const auto& ip : ip_pool) {
        std::stringstream ss;
        auto first_octet(true);
        for (const auto& ip_part : ip) {
        	if(!first_octet) {
                ss << '.';
        	}
            ss << ip_part;
            first_octet = false;
        }
        std::cout << ss.str() << std::endl;
    }
}


int main(int argc, char const* argv[])
{
    try {
	    std::basic_string<char> file_path;
        std::vector<std::vector<unsigned>> ip_pool(0);
    	
        std::getline(std::cin, file_path);
        std::ifstream ip_file(file_path);

        for (std::string line; std::getline(ip_file, line);) {
	        auto v = split(line, '\t');
            ip_pool.push_back(convert(split(v.at(0), '.')));
        }
        ip_file.close();

        std::sort(ip_pool.begin(), ip_pool.end(), [](const auto& ip_f, const auto& ip_s) {
            for (size_t i(0); i < ip_f.size(); ++i) {
                if (ip_f.at(i) != ip_s.at(i)) {
                    return ip_f[i] > ip_s[i];
                }  
            }
            return false;
            });

		// reverse lexicographically sort
        ip_print(ip_pool);
    	// filter by first byte and output
        ip_print(filter_ip_pool(ip_pool, unsigned(1)));
        // filter by first and second bytes and output
        ip_print(filter_ip_pool(ip_pool, unsigned(46), unsigned(70)));
        // filter by any byte and output
        ip_print(filter_ip_pool_any(ip_pool, 46));
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
