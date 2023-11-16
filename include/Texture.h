#ifndef TEXTURE_H
#define TEXTURE_H

#include <memory>
#include "Constants.h"
#include "Interval.h"
#include "Color.h"
#include "Vector3D.h"
#include "rtw_stb_image.h"

/**
 * @brief Describes the [0,1] u and v coordinates of a 2D texture.
 * 
 */
struct UnitTexturePosition {
    float_type u;
    float_type v;
};

/**
 * @brief Interface that describes what a given color should be for a point on a given texture.
 * 
 */
class Texture {
public:
    virtual ~Texture() = default;

    /**
     * @brief Returns the Color of the unit-plane-texture at point (u, v).
     * 
     * @param u the horizontal coordinate of the texture, where 0 is the leftmost point and 1 is the rightmost point.
     * @param v the vertical coordinate of the texture, where 0 is the bottommost point and 1 is the topmost point.
     * @param position The 3D point in space that the given (u, v) point on the texture will be mapped to. 
     * @return Color of the point
     */
    virtual Color value(float_type u, float_type v, const Vector3D& position) const = 0;
};

/**
 * @brief Describes a texture that is one uniform color.
 * 
 */
class SolidColorTexture : public Texture {
public:
    /**
     * @brief Construct a new Solid Color Texture object
     * 
     * @param color_ the uniform Color of the object.
     */
    SolidColorTexture(Color color_) : color {color_} {}

    /**
     * @brief Construct a new Solid Color Texture object
     * 
     * @param red the red component of the color
     * @param green the green component of the color
     * @param blue the blue component of the color
     */
    SolidColorTexture(float_type red, float_type green, float_type blue) : SolidColorTexture{Color{red, green, blue}} {}

    /**
     * @brief Returns the Color of the solid color unit-plane-texture at point (u, v).
     * 
     * @param u the (unused) horizontal coordinate of the texture, where 0 is the leftmost point and 1 is the rightmost point.
     * @param v the (unused) vertical coordinate of the texture, where 0 is the bottommost point and 1 is the topmost point.
     * @param position The (unused) 3D point in space that the given (u, v) point on the texture will be mapped to. 
     * @return Color of the point
     */
    Color value(float_type u, float_type v, const Vector3D& position) const override {
        (void) u, (void) v, (void) position;
        return color;
    }

private:
    Color color;
};


/**
 * @brief Describes a texture that two different textures, alternating betwen one and the other in a checkered pattern.
 * Which of the two textures is chosen is only dependant on the 3D position that the texture is located in space.
 * 
 */
class CheckerTexture : public Texture {
public:
    /**
     * @brief Construct a new Checker Texture object
     * 
     * @param scale_ Determines the size of the tiles in the checkered texture.
     * A value of 5 will result in 5x5x5 regions of textures.
     * @param even_ The first texture
     * @param odd_ The second texture
     */
    CheckerTexture(float_type scale_, std::shared_ptr<Texture> even_, std::shared_ptr<Texture> odd_) :
        inv_scale{1/scale_}, even{even_}, odd{odd_} 
    {}

    /**
     * @brief Construct a new Checker Texture object comprised of two solid colors.
     * 
     * @param scale Determines the size of the tiles in the checkered texture.
     * A value of 5 will result in 5x5x5 regions of textures. 
     * @param color1 The first color
     * @param color2 The second color
     */
    CheckerTexture(float_type scale, Color color1, Color color2) : 
        CheckerTexture{scale, std::make_shared<SolidColorTexture>(color1), std::make_shared<SolidColorTexture>(color2)}
    {}

    /**
     * @brief Returns the Color of the solid color unit-plane-texture at point (u, v).
     * 
     * @param u the horizontal coordinate of the texture, where 0 is the leftmost point and 1 is the rightmost point.
     * @param v the vertical coordinate of the texture, where 0 is the bottommost point and 1 is the topmost point.
     * @param position The 3D point in space that the given (u, v) point on the texture will be mapped to. 
     * @return Color of the point
     */
    Color value(float_type u, float_type v, const Vector3D& position) const override {
        //use floors rather than truncation to get consistent behavior across coordinate 0
        auto x_floor = static_cast<int>(std::floor(inv_scale * position.x()));
        auto y_floor = static_cast<int>(std::floor(inv_scale * position.y()));
        auto z_floor = static_cast<int>(std::floor(inv_scale * position.z()));

        bool is_even = (x_floor + y_floor + z_floor) % 2 == 0;
        return (is_even) ? even->value(u, v, position) : odd->value(u, v, position);
    }

private:
    float_type inv_scale;
    std::shared_ptr<Texture> even;
    std::shared_ptr<Texture> odd;
};


class ImageTexture : public Texture {
public:
    ImageTexture(const char* filename) : image{filename} {}

    Color value(float_type u, float_type v, const Vector3D& position) const override {
        //if we have no texture data, return solid cyan as a debugging aid
        if (image.height() <= 0) return Color{0, 1, 1};

        //clamp texture coordinates to [0, 1] x [1, 0]
        u = Interval{0, 1}.clamp(u);
        v = 1 - Interval{0, 1}.clamp(v);    //flip to image coordinates: 0 at top, 1 on bottom

        //convert [0, 1] coordinates u,v to image pixels
        auto i = static_cast<int>(u * image.width());
        auto j = static_cast<int>(v * image.height());
        auto pixel = image.pixel_data(i, j);

        //scale 8 bit rgb values to [0, 1] for Color constructor
        float_type color_scale = 1.0 / ColorConstants::max_pixel_val;
        return Color{color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]};
    }

private:
    rtw_image image;
};

#endif