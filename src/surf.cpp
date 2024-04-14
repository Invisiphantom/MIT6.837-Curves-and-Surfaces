#include "surf.h"
#include "vertexrecorder.h"
using namespace std;

namespace
{

    // We're only implenting swept surfaces where the profile curve is
    // flat on the xy-plane.  This is a check function.
    static bool checkFlat(const Curve &profile)
    {
        for (unsigned i = 0; i < profile.size(); i++)
            if (profile[i].V[2] != 0.0 ||
                profile[i].T[2] != 0.0 ||
                profile[i].N[2] != 0.0)
                return false;

        return true;
    }
}

// DEBUG HELPER
Surface quad()
{
    Surface ret;
    ret.VV.push_back(Vector3f(-1, -1, 0));
    ret.VV.push_back(Vector3f(+1, -1, 0));
    ret.VV.push_back(Vector3f(+1, +1, 0));
    ret.VV.push_back(Vector3f(-1, +1, 0));

    ret.VN.push_back(Vector3f(0, 0, 1));
    ret.VN.push_back(Vector3f(0, 0, 1));
    ret.VN.push_back(Vector3f(0, 0, 1));
    ret.VN.push_back(Vector3f(0, 0, 1));

    ret.VF.push_back({0, 1, 2});
    ret.VF.push_back({0, 2, 3});
    return ret;
}

Surface makeSurfRev(const Curve &profile, unsigned steps)
{
    if (!checkFlat(profile))
    {
        cerr << "surfRev profile curve must be flat on xy plane." << endl;
        exit(0);
    }

    Surface surface;
    for (unsigned p = 0; p < profile.size(); p++)
    {
        for (unsigned i = 0; i <= steps; i++)
        {
            float rad = float(i) / steps * 2 * M_PI;
            Matrix3f rMat = Matrix4f::rotateY(rad).getSubmatrix3x3(0, 0);
            Vector3f vVec = rMat * Vector3f(profile[p].V[0], profile[p].V[1], profile[p].V[2]);
            Vector3f nVec = rMat * profile[p].N;
            surface.VV.push_back(vVec);
            surface.VN.push_back(-1 * nVec);

            if (p != profile.size() - 1)
            {
                // 顺时针构造三角形
                unsigned index = p * (steps + 1) + i;
                unsigned next_index = index + (steps + 1);
                surface.VF.push_back({index, next_index, index + 1});
                surface.VF.push_back({index + 1, next_index, next_index + 1});
            }
        }
    }
    return surface;
}

Surface makeGenCyl(const Curve &profile, const Curve &sweep)
{
    if (!checkFlat(profile))
    {
        cerr << "genCyl profile curve must be flat on xy plane." << endl;
        exit(0);
    }

    // 计算初始法向量与终止法向量的夹角
    float alpha = -acos(Vector3f::dot(sweep.front().N, sweep.back().N));

    Surface surface;
    for (unsigned p = 0; p < profile.size(); p++)
    {
        for (unsigned i = 0; i < sweep.size(); i++)
        {
            // 插值旋转法向量
            auto rad_delta = alpha * i / (sweep.size() - 1);
            auto rotate_N = (cos(rad_delta) * sweep[i].N + sin(rad_delta) * sweep[i].B).normalized();
            auto rotate_B = (-sin(rad_delta) * sweep[i].N + cos(rad_delta) * sweep[i].B).normalized();
            Matrix4f rMat(rotate_N[0], rotate_B[0], sweep[i].T[0], sweep[i].V[0],
                          rotate_N[1], rotate_B[1], sweep[i].T[1], sweep[i].V[1],
                          rotate_N[2], rotate_B[2], sweep[i].T[2], sweep[i].V[2],
                          0, 0, 0, 1);
            Vector4f temp = rMat * Vector4f(profile[p].V[0], profile[p].V[1], profile[p].V[2], 1);
            Vector3f vVec = Vector3f(temp[0], temp[1], temp[2]);
            Matrix3f rMat_N = rMat.getSubmatrix3x3(0, 0).transposed().inverse();
            Vector3f nVec = rMat_N * profile[p].N;
            surface.VV.push_back(vVec);
            surface.VN.push_back(-1 * nVec);

            if (p != profile.size() - 1)
            {
                // 顺时针构造三角形
                unsigned index = p * sweep.size() + i;
                unsigned next_index = index + sweep.size();
                surface.VF.push_back({index, next_index, index + 1});
                surface.VF.push_back({index + 1, next_index, next_index + 1});
            }
        }
    }
    return surface;
}

void recordSurface(const Surface &surface, VertexRecorder *recorder)
{
    const Vector3f WIRECOLOR(0.4f, 0.4f, 0.4f);
    for (int i = 0; i < (int)surface.VF.size(); i++)
    {
        recorder->record(surface.VV[surface.VF[i][0]], surface.VN[surface.VF[i][0]], WIRECOLOR);
        recorder->record(surface.VV[surface.VF[i][1]], surface.VN[surface.VF[i][1]], WIRECOLOR);
        recorder->record(surface.VV[surface.VF[i][2]], surface.VN[surface.VF[i][2]], WIRECOLOR);
    }
}

void recordNormals(const Surface &surface, VertexRecorder *recorder, float len)
{
    const Vector3f NORMALCOLOR(0, 1, 1);
    for (int i = 0; i < (int)surface.VV.size(); i++)
    {
        recorder->record_poscolor(surface.VV[i], NORMALCOLOR);
        recorder->record_poscolor(surface.VV[i] + surface.VN[i] * len, NORMALCOLOR);
    }
}

void outputObjFile(ostream &out, const Surface &surface)
{

    for (int i = 0; i < (int)surface.VV.size(); i++)
        out << "v  "
            << surface.VV[i][0] << " "
            << surface.VV[i][1] << " "
            << surface.VV[i][2] << endl;

    for (int i = 0; i < (int)surface.VN.size(); i++)
        out << "vn "
            << surface.VN[i][0] << " "
            << surface.VN[i][1] << " "
            << surface.VN[i][2] << endl;

    out << "vt  0 0 0" << endl;

    for (int i = 0; i < (int)surface.VF.size(); i++)
    {
        out << "f  ";
        for (unsigned j = 0; j < 3; j++)
        {
            unsigned a = surface.VF[i][j] + 1;
            out << a << "/"
                << "1"
                << "/" << a << " ";
        }
        out << endl;
    }
}
