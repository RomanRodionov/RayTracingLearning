#ifndef SKYBOX_H
#define SKYBOX

#include "common.h"
#include "texture.h"
#include "vec3.h"

const double face_centers[6][2] = {{0.375, 0.5},
                                  {0.625, 0.5},
                                  {0.875, 0.5},
                                  {0.125, 0.5},
                                  {0.375, 1/6},
                                  {0.375, 5/6}};

class SkyBox
{
    public:
        shared_ptr<Texture> texture;
        SkyBox() {}
        SkyBox(const shared_ptr<Texture>& t) : texture(t) {}
        void load_texture(const shared_ptr<Texture>& t)
        {
            texture = t;
        }
        color get_color(const vec3& look_at_original)
        {
            const double edge = sqrt(2) / 2;
            vec3 look_at = -unit_vector(look_at_original);
            double x = look_at.x();
            double y = look_at.y();
            double z = look_at.z();
            double u = (look_at.x() + 1.0) * 0.5, v = (look_at.y() + 1.0) * 0.5;
            double tgv = y / sqrt(x*x+z*z);
            if (y <= edge && y >= -edge)
            {
                double tgh = z / x;
                if (z > 0 && fabs(z) >= fabs(x))
                {
                    u = face_centers[0][0] + 1.0 / tgh / 8;
                    v = face_centers[0][1] + tgv / 6;
                }
                else if (x > 0 && fabs(x) >= fabs(z))
                {
                    u = face_centers[1][0] - tgh / 8;
                    v = face_centers[1][1] + tgv / 6;
                }
                else if (z < 0 && fabs(z) >= fabs(x))
                {
                    u = face_centers[2][0] + 1.0 / tgh / 8;
                    v = face_centers[2][1] + tgv / 6;
                }
                else
                {
                    u = face_centers[3][0] - tgh / 8;
                    v = face_centers[3][1] + tgv / 6;
                }
            }
            return texture->get_color(u, v);
        }
};

#endif