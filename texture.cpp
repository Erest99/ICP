#include "texture.h"
#include <opencv2\opencv.hpp>

void bind_texture(GLuint shader, std::string path1, std::string path2, std::string path3, GLuint unit, GLenum tex)
{
    //TEXTURE INIT       
    int image_width1 = 0;
    int image_height1 = 0;
    int image_width2 = 0;
    int image_height2 = 0;
    int image_width3 = 0;
    int image_height3 = 0;
    const char* cpath1 = path1.c_str();
    const char* cpath2 = path2.c_str();
    const char* cpath3 = path3.c_str();
    unsigned char* image1 = SOIL_load_image(cpath1, &image_width1, &image_height1, NULL, SOIL_LOAD_RGBA);
    unsigned char* image2 = SOIL_load_image(cpath2, &image_width2, &image_height2, NULL, SOIL_LOAD_RGBA);
    unsigned char* image3 = SOIL_load_image(cpath3, &image_width3, &image_height3, NULL, SOIL_LOAD_RGBA);
    //unsigned char* image = SOIL_load_image("Images/brick.jpg", &image_width, &image_height, NULL, SOIL_LOAD_RGBA);

    GLuint texture0;
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);

    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    GLuint texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (image1 && image2 && image3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width1, image_height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width2, image_height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width3, image_height3, 0, GL_RGBA, GL_UNSIGNED_BYTE, image3);
        glGenerateMipmap(GL_TEXTURE_2D);

    }
    else
    {
        std::cout << "ERROR::TEXTURE_LOADING_FAILED " << "\n";
    }

    //glActiveTexture(0); //[GL CALLBACK] : source = API, type = ERROR, severity = HIGH, ID = '1280', message = 'GL_INVALID_ENUM error generated. <texture> exceeds the maximum supported number of combined texture image units.'
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image1);
    SOIL_free_image_data(image2);
    SOIL_free_image_data(image3);


    //Update uniforms
    glUniform1i(glGetUniformLocation(shader, "texture0"), unit);
    glUniform1i(glGetUniformLocation(shader, "texture1"), unit);
    glUniform1i(glGetUniformLocation(shader, "texture2"), unit);

    //Activate texture
    glActiveTexture(tex);
    glBindTexture(GL_TEXTURE_2D, texture0);

    //------------------------------------------------------------------------------------//
}

GLuint generate_texture(std::string path, GLuint unit, GLenum tex)
{
    //TEXTURE INIT       
    int image_width1 = 0;
    int image_height1 = 0;
    const char* cpath1 = path.c_str();
    unsigned char* image1 = SOIL_load_image(cpath1, &image_width1, &image_height1, NULL, SOIL_LOAD_RGBA);
    
    GLuint texture0;
    glGenTextures(1, &texture0);

    glActiveTexture(tex);
    glBindTexture(GL_TEXTURE_2D, texture0);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (image1)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width1, image_height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);
        glGenerateMipmap(GL_TEXTURE_2D);

    }
    else
    {
        std::cout << "ERROR::TEXTURE_LOADING_FAILED " << "\n";
    }
    /*
    glActiveTexture(tex);
    glBindTexture(GL_TEXTURE_2D, texture0);
    */
    SOIL_free_image_data(image1);
    return texture0;
}

static void textureLoad(cv::Mat& image, const bool transparent, bool compressed = true)
{
    if (compressed && glewIsSupported("GL_ARB_texture_compression"))
    {
        GLint num_compressed_format;

        // get list of supported formats
        glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB, &num_compressed_format);
        if (num_compressed_format > 0)
        {
            GLint internalformat, compressed_size;
            int compressed;

            std::cout << "COMPRESSION supported, available formats: " << num_compressed_format << std::endl;

            // try to load compressed texture
            glHint(GL_TEXTURE_COMPRESSION_HINT, GL_FASTEST);
            //glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);

            if (transparent)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_ARB, image.cols, image.rows, 0, GL_BGRA, GL_UNSIGNED_BYTE, image.data);
            }
            else {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_ARB, image.cols, image.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, image.data);
            }

            // is it really compressed?
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_ARB, &compressed);
            /* if the compression has been successful */
            if (compressed == GL_TRUE)
            {
                glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalformat);
                glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE_ARB, &compressed_size);
                std::cout << "ORIGINAL: " << image.total() * image.elemSize() << " COMPRESSED: " << compressed_size << " INTERNAL FORMAT: " << internalformat << std::endl;
            }
        }
    }
    else
    {
        // load uncompressed
        if (transparent)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.cols, image.rows, 0, GL_BGRA, GL_UNSIGNED_BYTE, image.data);
        }
        else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, image.data);
        }
    }
}

GLuint textureInit(const char* cesta, const bool mirror, const bool transparent)
{
    GLuint texture;
    cv::Mat image;

    if (transparent) {
        image = cv::imread(cesta, cv::IMREAD_UNCHANGED);
    }
    else {
        image = cv::imread(cesta);
    }

    if (image.empty())
    {
        std::cerr << "no texture: " << cesta << std::endl;
        exit(1);
    }

    glGenTextures(1, &texture);
    std::cout << "TexID: " << texture << std::endl;

    glBindTexture(GL_TEXTURE_2D, texture);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    textureLoad(image, transparent, true);

   // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//[GL CALLBACK]: source = API, type = ERROR, severity = HIGH, ID = '1280', message = 'GL_INVALID_ENUM error generated. <target> or <pname> require feature(s) disabled in the current profile.'

    if (mirror) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    }
    else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    // bilinear - better & slower
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
}