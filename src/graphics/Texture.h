//
//  Texture.h
//  dryGL
//
//  Created by Jordi Ros on 15/02/13.
//  Copyright (c) 2013 Jordi Ros. All rights reserved.
//

#pragma once

namespace dry {

class Texture
{
public:
    struct Params
    {
        Params()
        {
            Bilinear = true;
            Mipmaps  = false;
        }
        Params(bool bilinear, bool mipmaps)
        {
            Bilinear = bilinear;
            Mipmaps  = mipmaps;
        }
        bool    Bilinear;
        bool    Mipmaps;
    };
public:
                Texture         () { m_Handle = -1; }
               ~Texture         () { Free(); }

    bool        InitWithData    (int width, int height, PixelFormat format, Params const &params, void const *data);
    void        Free            ();

    void        Update          (void const *data);

    void        Bind            (int stage) const;
    void        Unbind          () const;
    
private:
    
    int         GetGLFormat     () const;
    int         GetGLType       () const;
    
private:
    
    int         m_Handle;
    int         m_Width;
    int         m_Height;
    PixelFormat m_Format;
    Params      m_Params;
    int         m_Target;
};

}