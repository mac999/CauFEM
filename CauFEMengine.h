#ifndef __CAU_FEM_ENGINE_H__
#define __CAU_FEM_ENGINE_H__
// ���� : FEM ����
// �ۼ��� : ���¿�
// �ۼ��� : 2005.7.24

#include <AfxTempl.h>

// Constance define 
//
#define MAX_DEF_STR	256
#define DEF_METRIX_SIZE 6	// �ӽ÷� 3d truss�� ����.
#define FEM_ERROR_VALUE -1.

#define _BooLeq(c1, c2)	(fabs((c1) - (c2)) < 1.0E-6)
#define _BooLz(v1)		(fabs(v1) < 1.0E-6)
#define _POW2(v)	(v * v)

// FemVector class
//
#define MAX_DIM 3
class FemVector
{
public:
	FemVector();
	~FemVector();

public:
	double v[MAX_DIM];
};


// FemMetrix class
//
class FemMetrix
{
public:
	FemMetrix(int iSize = DEF_METRIX_SIZE);
	virtual ~FemMetrix();

	int GetElementSize();

	void SetAt(int i, int j, double dValue);
	double GetAt(int i, int j);

	FemMetrix& operator*=(double dScalar);
	FemVector& operator*(FemVector& v);

	void CopyFrom(double aaMetrix[DEF_METRIX_SIZE][DEF_METRIX_SIZE]);
	void SetSymmetry();
	void Inverse();	// K^-1

private:
	int m_iSize;

	#define MAX_METRIX_SIZE 128
	double* m_pMetrix[MAX_METRIX_SIZE];
};


// FemFullMetrix class
//
class FemEngine;
class FemFullMetrix : public FemMetrix
{
public:
	FemFullMetrix();
	virtual ~FemFullMetrix();

	bool Add(FemEngine* pEngine, FemMetrix& mK);

	void Reduce(FemEngine* pEngine, FemVector& vDelta);
};


// FemVertex class
//
class FemVertex
{
public:
	FemVertex(FemEngine* pEngine);
	~FemVertex();

	void SetFixed(int iIndex, bool bFixed);
	bool IsFixed(int iIndex);
	void SetCoord(int iIndex, double dValue);
	double GetCoord(int iIndex);
	void SetForce(int iIndex, double dForce);
	double GetForce(int iIndex);

	double GetLength(FemVertex* pVertex);
	enum AxisPlaneType
	{
		eAxisX = 0,
		eAxisY,
		eAxisZ
	};
	double GetCosineAt(int iAxis, FemVertex* pVertex);

	virtual void Dump();

protected:
	bool m_abFixed[MAX_DIM];	// ��������
	double m_aCoord[MAX_DIM];	// ��ǥ
	double m_aForce[MAX_DIM];	// �ܷ�
	// double m_aMoment[MAX_DIM];	// 

	FemEngine* m_pEngine;
};

// class FemElement
//
class FemElement
{
public:
	FemElement(FemEngine* pEngine);
	~FemElement();

	enum MaturialType
	{
		eElastic = 0,	// ź��
		eArea = 1,		// ����
		eMaxMaturial	
	};
	void SetMaturial(FemElement::MaturialType eType, double dValue);
	virtual bool GetK(FemMetrix& mK);
	virtual bool GetForce(FemVector& vForce);

	virtual void Dump();

protected:
	double m_adMaturial[FemElement::eMaxMaturial];
	FemMetrix m_m;

	FemEngine* m_pEngine;
};

// FemBeamElement class
//
// class FemBeamElement : public FemElement
// {
// public:
// 	virtual bool GetK(FemMetrix& mK);
// };

// FemBarElement class
//
// #define MAX_BAR_VERTEX 2;
// class FemBarElement : public FemElement		// ��
// {
// public:
// 	virtual bool GetK(FemMetrix& mK);
// 
// private:
// 	int m_aVertex[MAX_BAR_VERTEX];
// };

// FemBeamElement class
//
// class FemBeamElement : public FemElement	// ��
// {
// public:
// 	virtual bool GetK(FemMetrix& mK);
// };
// 

// FemTrussElement class
//
class FemTrussElement : public FemElement	// Ʈ����Ʈ
{
public:
	FemTrussElement(FemEngine* pEngine);

	void SetVertex(int iNo, int iVertex);
	int GetVertexIndex(int iNo);
	FemVertex* GetVertex(int iNo);

	double GetLength();
	double GetCosineAt(int iAxis);

	virtual bool GetK(FemMetrix& mK);

private:
	double m_dArea;
	int m_aVertexIndex[2];
};

// FemSolid class
//
// class FemSolidElement : public FemElement
// {
// public:
// 	FemSolid();
// 	
// 	void DivideElement();	// ��Һ���
// }
 
// FemEngine class
//
#define MAX_FEM_VERTEX  200
#define MAX_FEM_ELEMENT 100
class FemEngine	// Vertex ���. 2���� ������ ���� �������.
{
public:
	FemEngine();
	~FemEngine();

	bool Init();
	bool Load(const char* szInputFile);
	void Action();
	void Save(const char* szOutputFile);

	bool AddVertex(FemVertex* pVertex);
	int GetVertexCount();
	FemVertex* GetVertexAt(int iIndex);
	void ClearVertex();

	bool AddElement(FemElement* pEle);
	int GetElementCount();
	FemElement* GetElementAt(int iIndex);
	void ClearElement();

protected:
	virtual bool Solve(FemFullMetrix& FullMetrix, FemVector& vDelta);
	virtual void GetForce(FemFullMetrix& mK, FemVector& vDelta, FemVector &vForce);	// F = Kd ���� d�� ���

private:
	CString m_sTitle;
	CArray<FemVertex*, FemVertex*> m_aVertex;
	CArray<FemElement*, FemElement*> m_aElement;
};


// �۾� ��ȹ
//
// 1. ���� p154�� ������ Ʈ���� FEM�� �����.
// 2. 1 �ϼ��� �̸� ������� ��� ������/���� �ؼ� FEM�� �����.
// 3. 2D �ﰢ�� �������� FEM�� �����.
//
  
#endif
