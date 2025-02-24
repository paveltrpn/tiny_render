
#include <fstream>
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <string>

#include "tga.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_IMAGE_TGA_CPP{ false };

namespace tire {

Tga::Tga( std::string_view fname ) {
    std::ifstream fileStream;
    unsigned char BlockInfo, isPacked, R, G, B, A;
    unsigned int NumPixels;
    size_t i, k;

    fileStream.open( fname, std::ifstream::binary );
    if ( !fileStream ) {
        throw std::runtime_error(
            std::format( "file \"{}\" not found", std::string{ fname } ) );
    }

    fileStream.read( reinterpret_cast<char *>( &sTGAHeader_ ),
                     sizeof( STGAHeader ) );

    decompressed_ = new uint8_t[sTGAHeader_.width * sTGAHeader_.height *
                                ( sTGAHeader_.bits / 8 )];

    // Non RLE
    if ( sTGAHeader_.imageType == 2 ) {
        for ( i = 0; i < sTGAHeader_.width * sTGAHeader_.height; i++ ) {
            fileStream.read( reinterpret_cast<char *>( &R ), 1 );
            fileStream.read( reinterpret_cast<char *>( &G ), 1 );
            fileStream.read( reinterpret_cast<char *>( &B ), 1 );

            if ( ( sTGAHeader_.bits / 8 ) == 4 ) {
                fileStream.read( reinterpret_cast<char *>( &A ), 1 );
            }

            decompressed_[i * ( sTGAHeader_.bits / 8 ) + 0] = B;
            decompressed_[i * ( sTGAHeader_.bits / 8 ) + 1] = G;
            decompressed_[i * ( sTGAHeader_.bits / 8 ) + 2] = R;

            if ( ( sTGAHeader_.bits / 8 ) == 4 ) {
                decompressed_[i * ( sTGAHeader_.bits / 8 ) + 3] = A;
            }
        }
    }

    // RLE
    if ( sTGAHeader_.imageType == 10 ) {
        for ( i = 0; i < sTGAHeader_.width * sTGAHeader_.height; ) {
            fileStream.read( reinterpret_cast<char *>( &BlockInfo ), 1 );

            isPacked = BlockInfo & 128;
            NumPixels = BlockInfo & 127;

            // Если запакованные данные
            if ( isPacked ) {
                fileStream.read( reinterpret_cast<char *>( &R ), 1 );
                fileStream.read( reinterpret_cast<char *>( &G ), 1 );
                fileStream.read( reinterpret_cast<char *>( &B ), 1 );

                if ( ( sTGAHeader_.bits / 8 ) == 4 ) {
                    fileStream.read( reinterpret_cast<char *>( &A ), 1 );
                }

                for ( k = 0; k < NumPixels + 1; k++ ) {
                    decompressed_[i * ( sTGAHeader_.bits / 8 ) + 0] = B;
                    decompressed_[i * ( sTGAHeader_.bits / 8 ) + 1] = G;
                    decompressed_[i * ( sTGAHeader_.bits / 8 ) + 2] = R;

                    if ( ( sTGAHeader_.bits / 8 ) == 4 ) {
                        decompressed_[i * ( sTGAHeader_.bits / 8 ) + 3] = A;
                    }
                    i++;
                }

            } else {  // Если незапакованные
                for ( k = 0; k < NumPixels + 1; k++ ) {
                    fileStream.read( reinterpret_cast<char *>( &R ), 1 );
                    fileStream.read( reinterpret_cast<char *>( &G ), 1 );
                    fileStream.read( reinterpret_cast<char *>( &B ), 1 );

                    if ( ( sTGAHeader_.bits / 8 ) == 4 ) {
                        fileStream.read( reinterpret_cast<char *>( &A ), 1 );
                    }

                    decompressed_[i * ( sTGAHeader_.bits / 8 ) + 0] = B;
                    decompressed_[i * ( sTGAHeader_.bits / 8 ) + 1] = G;
                    decompressed_[i * ( sTGAHeader_.bits / 8 ) + 2] = R;

                    if ( ( sTGAHeader_.bits / 8 ) == 4 ) {
                        decompressed_[i * ( sTGAHeader_.bits / 8 ) + 3] = A;
                    }

                    i++;
                }
            }
        }
    }

    fileStream.close();
}

Tga::~Tga() {
    delete[] decompressed_;
}

size_t Tga::widht() {
    return sTGAHeader_.width;
}

size_t Tga::height() {
    return sTGAHeader_.height;
}

size_t Tga::chanels() {
    return sTGAHeader_.bits / 8;
}

uint8_t *Tga::data() {
    return decompressed_;
}

}  // namespace tire
