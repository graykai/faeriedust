#include "JzAzBzColor.h"


double lms(double t) {
    if (t > 0.) {
        double r = pow(t, 0.007460772656268214);
        double s = (0.8359375 - r) / (18.6875 * r + -18.8515625);
        return pow(s, 6.277394636015326);
    }
    else {
        return 0.;
    }
}

double srgb(double c) {
    if (c <= 0.0031308049535603713) {
        return c * 12.92;
    }
    else {
        double c_ = pow(c, 0.41666666666666666);
        return c_ * 1.055 + -0.055;
    }
}

glm::vec3 JzAzBzColor::to_srgb() {
   return xyz_to_srgb(value.x, value.y, value.z);
}

glm::vec3 xyz_to_srgb(float x, float y, float z) {
    double jz = x * 0.16717463120366200 + 1.6295499532821566e-11;
    double cz = y * 0.16717463120366200;
    double hz = z * 6.28318530717958647 + -3.14159265358979323;

    double iz = jz / (0.56 * jz + 0.44);
    double az = cz * cos(hz);
    double bz = cz * sin(hz);

    double l_ = iz + az * +0.13860504327153930 + bz * +0.058047316156118830;
    double m_ = iz + az * -0.13860504327153927 + bz * -0.058047316156118904;
    double s_ = iz + az * -0.09601924202631895 + bz * -0.811891896056039000;

    double l = lms(l_);
    double m = lms(m_);
    double s = lms(s_);

    double lr = l * +0.0592896375540425100e4 + m * -0.052239474257975140e4 + s * +0.003259644233339027e4;
    double lg = l * -0.0222329579044572220e4 + m * +0.038215274736946150e4 + s * -0.005703433147128812e4;
    double lb = l * +0.0006270913830078808e4 + m * -0.007021906556220012e4 + s * +0.016669756032437408e4;

    return glm::vec3(srgb(lr), srgb(lg), srgb(lb));
}

ofColor jab_to_of(float j, float a, float b) {
    glm::vec3 srgb = xyz_to_srgb(j, a, b) * 255.0;
    srgb = glm::clamp(srgb, { 0., 0., 0. }, { 255., 255., 255. });
    return { (float)srgb[0], (float)srgb[1], (float)srgb[2] };
}