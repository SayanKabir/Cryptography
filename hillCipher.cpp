#include <bits/stdc++.h>
using namespace std;

//helper function
void printMatrix(vector<vector<int>> arr)
{
    int n = arr.size();
    int m = arr[0].size();
    for (int i = 0; i < arr.size(); i++)
    {
        for (int j = 0; j < arr[i].size(); j++)
        {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

//IO formatting
std::ostream &bold_on(std::ostream &os)
{
    return os << "\e[1m";
}
std::ostream &bold_off(std::ostream &os)
{
    return os << "\e[0m";
}

//HILL CIPHER
class HillCipher
{
private:
protected:
    string m_PlainText;
    string m_CipherText;
    string m_Key;
    enum operationType
    {
        encryption,
        decryption
    };
    int getCorrNum(char ch)
    {
        return ch - 'A';
    }
    char getBackAlphabet(int n)
    {
        return 'A' + n;
    }
    void resetValues()
    {
        m_PlainText = "";
        m_CipherText = "";
        m_Key = "";
    }
    void fillMatrices(string &pt, string &key, vector<vector<int>> &KEYMAT, vector<vector<int>> &PTMAT, int l, int m)
    {
        int k = 0;
        int ksize = key.size();
        for (int col = 0; col < m; col++)
        {
            for (int row = 0; row < m; row++)
            {
                KEYMAT[row][col] = getCorrNum(key[k++]);
                if (k == ksize)
                    k = 0;
            }
        }
        int p = 0;
        int psize = pt.size();
        for (int row = 0; row < l; row++)
        {
            for (int col = 0; col < m; col++)
            {
                if (p < psize)
                    PTMAT[row][col] = getCorrNum(pt[p++]);
                else
                    PTMAT[row][col] = 25;
            }
        }
    }

    //for finding inverse of key matrix
    class MatInverse
    {
        private:
        static inline int mod26(int x)
        {
            return (x>=0)?(x%26):(26-(abs(x)%26));
        }
        static int findDeterminant(vector<vector<int>> m, int n)
        {
            int det;
            if(n == 2)
            {
                det = m[0][0] * m[1][1] - m[0][1]*m[1][0] ;
            }
            else if (n == 3)
            {
                det = m[0][0]*(m[1][1]*m[2][2] - m[1][2]*m[2][1])  - m[0][1]*(m[1][0]*m[2][2] - m[2][0]*m[1][2] ) + m[0][2]*(m[1][0]*m[2][1] - m[1][1]*m[2][0]);
            }
            else det = 0 ; // invalid input
            return mod26(det);
        }
        static int findDeterminantInverse(int R , int D = 26) // R is the remainder or determinant
        {
            int i = 0 ;
            int p[100] = {0,1};
            int q[100] = {0} ; // quotient

            while(R!=0)
            {
                q[i] = D/R ;
                int oldD = D ;
                D = R ;
                R = oldD%R ;
                if(i>1)
                {
                    p[i] = mod26(p[i-2] - p[i-1]*q[i-2]) ;
                }
                i++ ;
            }
            if (i == 1) return 1;
            else return p[i] = mod26(p[i-2] - p[i-1]*q[i-2]) ;
        }
        public:
        static void findInverse(vector<vector<int>> m , int n , vector<vector<int>>& m_inverse)
        {
            vector<vector<int>> adj(3, vector<int>(3, 0));

            int det = findDeterminant(m , n);
            if(det==0)
                cout<<"[!] Should not encrypt/decrypt with a non-invertible key"<<endl;
            int detInverse = findDeterminantInverse(det);
            if(n==2)
            {
                adj[0][0] = m[1][1];
                adj[1][1] = m[0][0];
                adj[0][1] = -m[0][1];
                adj[1][0] = -m[1][0];
            }
            else if(n==3)
            {
                vector<vector<int>> temp(5, vector<int>(5, 0));
                for(int i=0; i<5; i++)
                {
                    for(int j=0; j<5; j++)
                    {
                        temp[i][j] = m[i%3][j%3] ;
                    }
                }
                for(int i=1; i<=3 ; i++)
                {
                    for(int j=1; j<=3 ; j++)
                    {
                        adj[j-1][i-1] = temp[i][j]*temp[i+1][j+1] - temp[i][j+1]*temp[i+1][j];
                    }
                }
            }

            for(int i=0; i<n ; i++)
            {
                for(int j=0; j<n ; j++)
                {
                    m_inverse[i][j] = mod26(adj[i][j] * detInverse) ;
                }
            }
        }
    };
    vector<vector<int>> multiplyMatrices(vector<vector<int>> PTMAT, vector<vector<int>> KEYMAT)
    {
        int l = PTMAT.size();
        int m = KEYMAT.size();
        vector<vector<int>> res(l, vector<int>(m, 0));
        for (int row = 0; row < l; row++)
        {
            for (int col = 0; col < m; col++)
            {
                for (int k = 0; k < m; k++)
                {
                    res[row][col] += PTMAT[row][k] * KEYMAT[k][col];
                }
                res[row][col] = res[row][col] % 26;
            }
        }
        return res;
    }

    //Input and Output
    void getInput(operationType type)
    {
        if (type == encryption)
        {
            cout << "Enter" << bold_on << " Plain Text: " << bold_off;
            cin >> m_PlainText;
            m_PlainText.erase(remove(m_PlainText.begin(), m_PlainText.end(), ' '), m_PlainText.end());
            transform(m_PlainText.begin(), m_PlainText.end(), m_PlainText.begin(), ::toupper);
        }
        else if (type == decryption)
        {
            cout << "Enter" << bold_on << " Cipher Text: " << bold_off;
            cin >> m_CipherText;
            m_CipherText.erase(remove(m_CipherText.begin(), m_CipherText.end(), ' '), m_CipherText.end());
            transform(m_CipherText.begin(), m_CipherText.end(), m_CipherText.begin(), ::toupper);
        }
        cout << "Enter" << bold_on << " KEY " << bold_off << "for encryption: ";
        cin >> m_Key;
        m_Key.erase(remove(m_Key.begin(), m_Key.end(), ' '), m_Key.end());
        transform(m_Key.begin(), m_Key.end(), m_Key.begin(), ::toupper);
    }
    void showResult(operationType type)
    {
        if (type == encryption)
        {
            cout << bold_on << "Cipher Text: " << m_CipherText << bold_off;
        }
        else if (type == decryption)
        {
            cout << bold_on << "Plain Text: " << m_PlainText << bold_off;
        }
        cout<<endl;
    }

public:
    void encrypt()
    {
        resetValues();
        getInput(encryption);
        int m = ceil(sqrt(m_Key.size()));
        int l;
        if (m_PlainText.size() % m == 0)
            l = m_PlainText.size() / m;
        else
            l = m_PlainText.size() / m + 1;
        vector<vector<int>> KEYMAT(m, vector<int>(m));
        vector<vector<int>> PTMAT(l, vector<int>(m));
        vector<vector<int>> CTMAT(l, vector<int>(m));
        fillMatrices(m_PlainText, m_Key, KEYMAT, PTMAT, l, m);

        CTMAT = multiplyMatrices(PTMAT, KEYMAT);
        for (int row = 0; row < l; row++)
        {
            for (int col = 0; col < m; col++)
            {
                m_CipherText += getBackAlphabet(CTMAT[row][col]);
            }
        }
        
        showResult(encryption);
    }
    void decrypt()
    {
        resetValues();
        getInput(decryption);
        int m = ceil(sqrt(m_Key.size()));
        int l;
        if (m_CipherText.size() % m == 0)
            l = m_CipherText.size() / m;
        else
            l = m_CipherText.size() / m + 1;
        vector<vector<int>> KEYMAT(m, vector<int>(m));
        vector<vector<int>> PTMAT(l, vector<int>(m));
        vector<vector<int>> CTMAT(l, vector<int>(m));
        fillMatrices(m_CipherText, m_Key, KEYMAT, CTMAT, l, m);

        vector<vector<int>> KEYINVERSEMAT(m, vector<int>(m));
        MatInverse::findInverse(KEYMAT, m, KEYINVERSEMAT);

        PTMAT = multiplyMatrices(CTMAT, KEYINVERSEMAT);
        for (int row = 0; row < l; row++)
        {
            for (int col = 0; col < m; col++)
            {
                m_PlainText += getBackAlphabet(PTMAT[row][col]);
            }
        }

        showResult(decryption);
    }
};

int main()
{
    HillCipher myCipher;
    myCipher.encrypt();
    myCipher.decrypt();
    return 0;
}