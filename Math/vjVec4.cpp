#include <config.h>
#include <Math/vjVec4.h>

void vjVec4::xform(const vjMatrix& _m, vjVec4 _v)
{
	// Take _v by value in case it is me
	// Treating Vector like a column vector
	// (4x4)(4x1) = (4x1)
    
    vec[0] = vec[1] = vec[2] = vec[3] = 0.0f;
    
    //for(int i=0;i<1;i++)
	for(int j=0;j<4;j++)
	    for(int k=0;k<4;k++)
		vec[j] += (_v[k] * _m[k][j]);
}

inline vjVec4 operator *(float _s, const vjVec4& _v) {
    return vjVec4(_v[0]*_s, _v[1]*_s, _v[2]*_s, _v[3]*_s);
}

inline vjVec4 operator *(const vjVec4& _v, float _s) {
    return vjVec4(_v[0]*_s, _v[1]*_s, _v[2]*_s, _v[3]*_s);
}

inline vjVec4 operator /(const vjVec4& _v, float _s) {
    _s = 1.0f/_s;
    return vjVec4(_v[0]*_s, _v[1]*_s, _v[2]*_s, _v[3]*_s);
}


ostream& operator<<(ostream& out, vjVec4& _v)
{
    for(int j=0;j<4;j++)
    {
    	out << _v.vec[j] << ", ";
    }
    
    return out;	   
}
