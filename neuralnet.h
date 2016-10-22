//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

using namespace std;

class NeuralNet
{
public:
// Данные нейронной сети
        long m_lWeightsCount;
        long m_lNodesCount;
        long m_lLayersCount;

		double * m_pdWeights; // веса
        double * m_pdNodes; // активации нейронов
		long * m_plLayers; // слои

// Данные для обучения/тестирования
        long m_lPatternsCount;
        long m_lTestPatternsCount;
        double * m_pdPatterns;

// Дополнительные сервисные функции состояния сети
        double GetRelevance(long lNode);
//        CString GetNMExpression(long lNode);

private:
// Вспомогательные массивы
        long * m_plNodePtr;
        long * m_plWeightPtr;
        double * m_pdWeightsOld;
        double * m_pdGradient;
        double * m_pdOldGradient;
        double * m_pdDelta;
        double * m_pdOldDelta;
        double * m_pdDerivatives;
        double * m_pdErrors;

public:
        NeuralNet();
        virtual ~NeuralNet();

// Работа с данными
        void CreatePatterns(long numPatterns);
        void SetPattern(long Pattern, long Unit, double Value);

// Создание нейросети
        int CreateNetwork(string strInitString);
		int CreateNetwork(long nLayersCount, long *nLayers);
        void InitializeNetwork(unsigned int RandomSeed);

// Обучение нейросети
        void TrainBPROP(long Length, double MaxError, double LearningRate=0.5, double Momentum=0.1);
        void TrainRPROP(long Length, double MaxError, double IncreaseFactor=1.2, double DecreaseFactor=0.5, double DeltaMin=1e-6, double DeltaMax=50.0, double DeltaInit=0.1);
        void TrainSCG  (long Length, double MaxError, double Sigma=1e-4, double Lambda=1e-6);
		void Test(long Pattern);

// Прочее ............................ разобраться
        double m_dTolerance; // НЕпонятно пока .................

protected:
// Системные функции
		void ClearNetwork();
        void FreeNetworkMemory();

private:
// Вспомогательные математические функции
		inline double sign(double a) {if (a > 0) return 1.0; if (a < 0) return -1.0; return 0.0;};
		inline double sign1(double a) {if (a >= 0) return 1.0; return -1.0;};
		inline double product_of_xt_by_y(double *x, double *y, int tab_size){ double sum = 0 ; for (long indice = 0 ; indice < tab_size ; indice++) sum += x[indice]*y[indice] ; return(sum);}
		inline double square_of_norm(double *x, int tab_size){return(product_of_xt_by_y(x,x,tab_size));}
// Вспомогательные функции обучения
        inline double BackPropagate(double *pWeights);
		double CalcErrorWithoutUnit(int iNumUnit);
};

