//
// Name :         GrImageI.h
// Description :  Header for CGrImageI, a general purpose image class.
//                This is similar to CGrImage, but has int pixels instead of BYTE

#pragma once

#if !defined(_GRIMAGEI_H)
#define _GRIMAGEI_H


class CGrImageI
{
public:
    //! Default Constructor
    CGrImageI();
    
    CGrImageI(const CGrImageI &p_img);

    //! Destructor
    virtual ~CGrImageI();

    //! Clear the image 
    /*! Clears the image and releases any memory. */
    void Clear();

    //! Fill the image with a color
    /*! Fills the image with a single color. The 
        values for R, G, B, and A are in the range 0 to 255.
        \param r Red component or gray component for monochrome image.
        \param g Green component (optional).
        \param b Blue component (optional). 
        \param a Alpha component (optional). */
    void Fill(int r, int g=0, int b=0, int a=0);

    //! Set the image size
    /*! Sets the image size. THis will clear the image if it
        has already been loaded. 
        \param wid New image width 
        \param hit New image height 
        \param planes Number of color planes. Default, if not supplied 3 is used. */
    void SetSize(int wid, int hit, int planes=3);


    //! Set a pixel value
    /*! Sets the value of a pixel. 
        \param x X location (column)
        \param y Y location (row)
        \param r Red component or gray component for monochrome image.
        \param g Green component (optional).
        \param b Blue component (optional).
        \param a Alpha component (optional). */
    void Set(int x, int y, int r, int g=0, int b=0, int a=0);

    //! Set this image to be the same size as another
    /*! Sets the image size identical to that of another
        image. If the optional number of planes is supplied,
        the new image is set to that number of color planes.
        It not, the image is set to the same number of color 
        planes as the other image.
        \param img The other image */
    void SetSameSize(const CGrImageI &img, int planes=-1);

    //! Copies an image into this one.
    void Copy(const CGrImageI &p_img);

    //! Returns true if the texture image is empty.
    bool IsEmpty() const;

    //! Assignment operator that copies a texture image into this one.
    CGrImageI &operator=(const CGrImageI &p_img);

    //! Bracket operator gets access to a given row of the image
    /*! This operator can be used like this:  \code
    BYTE value = texture[row][byte];
\endcode
        Use with care. The return value is that same as that for
        ImageBits. It can be used to directly access the 
        data for the texture image. It consists of a sequence of 
        bytes in the order B, G, and R. Rows start on the texture
        word (32 bit) boundary. */
    int *operator[](int i);

    //! Bracket operator gets access to a given row of the image
        /*! This operator can be used like this:  \code
    BYTE value = image[row][arraycolumn];
\endcode
        Use with care. The return value is that same as that for
        Row. It can be used to directly access the 
        data for the texture image. It consists of a sequence of 
        bytes in the order B, G, and R for the given row. */
    const int *operator[](int i) const;

    //! Access to pixels in a given row of the image
        /*! This operator can be used like this:  \code
    BYTE *row = image.Row(row);
    BYTE red = row[column * 3 + 2];
\endcode
        Use with care. It can be used to directly access the 
        data for the image. It consists of a sequence of 
        bytes in the order B, G, and R for the given row. */
    int *GetRow(int i);

    //! Access to pixels in a given row of the image
    /*! This operator can be used like this:  \code
    const BYTE *row = image.GetRow(row);
    BYTE red = row[column * 3 + 2];
\endcode
        Use with care. It can be used to directly access the 
        data for the image. It consists of a sequence of 
        bytes in the order B, G, R, and optionally A for the given row. */
    const int *GetRow(int i) const;

    //! Gets the texture image width
    int GetWidth() const;

    //! Gets the texture image height
    int GetHeight() const;

    //! Direct access to the image bits. 
    /*! This function can be used to directly access the 
        data for the image data. It consists of a sequence of 
        bytes in the order B, G, and R. Rows start on a
        word (32 bit) boundary. */
    int *GetImageBits() const;

    //! Get the number of color planes
    //! \return The number of color planes. Can be 1, 3, or 4
    int GetPlanes() const {return m_planes;}

    //! Get the row pitch
    //! \return The row pitch. This is the number of bytes from
    //! the start of one row to the start of the next.
    int GetRowPitch() const {return m_rowpitch;}


private:
    // Number of image planes (colors)
    int m_planes;

    // Number of bytes per row
    int m_rowpitch;

    // Image height
    int m_height;

    // Image width
    int m_width;

    // The actual image data
    int ** m_image;
};

#endif 
