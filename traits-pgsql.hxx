#ifndef TRAITS_PGSQL_HXX
#define TRAITS_PGSQL_HXX

#include <cstddef> // std::size_t
#include <cstring> // std::strncmp, std::memset, std::memcpy
#include <ctime>   // localtime, mktime, time_t, tm
#include <string>
#include <iostream>

#include <odb/pgsql/traits.hxx>
#include <odb/pgsql/details/endian-traits.hxx>

using namespace std;

namespace odb
{
    namespace pgsql
    {

        template <>
        class value_traits<double, pgsql::id_numeric> {
        public:
            typedef double value_type;
            typedef double query_type;
            typedef details::buffer image_type;

            static int
            dataToInt(const char * data, int a, int b) {
                return (((unsigned char)data[a]) * 256 + ((unsigned char)data[b]));
            }

            static void
            set_value (double& v,
                       const details::buffer& b,
                       std::size_t n,
                       bool is_null)
            {
                //v = (!is_null && n == 4 && std::strncmp ("true", b.data (), n) == 0);
                if(is_null)
                    v = 0.0;
                else {
                    // int p1 = dataToInt(b.data(), 0, 1);
                    int p2 = dataToInt(b.data(), 2, 3);
                    int p3 = dataToInt(b.data(), 4, 5);
                    // int p4 = dataToInt(b.data(), 6, 7);

                    string snumber = (p3 == 16384? "-": "");
                    for(unsigned int i = 8; i < n; i += 2) {
                        char buff[5];
                        sprintf(buff, "%04d", dataToInt(b.data(), i, i + 1));
                        snumber.append(buff);
                        if((int(i) - 8)/2 == p2)
                            snumber.append(".");
                    }

                    if(snumber.empty())
                        v = 0.0;
                    else {
                        v = stod(snumber);
                    }
                }
            }

            static void
            set_image (details::buffer& b,
                       std::size_t& n,
                       bool& is_null,
                       double v)
            {
                is_null = false;
                n = 9 * v;

                if (n > b.capacity ())
                    b.capacity (n);

                std::memcpy (b.data (), "7685.7685", n);
                v = 0.0;
            }
        };
    }
}

#endif // TRAITS_PGSQL_HXX
