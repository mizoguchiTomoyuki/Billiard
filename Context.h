template<int N>
class Context{
public:
	char str[N];
private:
	int size;
	int pos;
public:
	Context() :size(N), pos(){} //�R���X�g���N�^�Ńt�B�[���h�Ŏw�肵���ϐ���������

	//�p�����[�^���擾���ă|�C���^���ɐi�߂�
	std::string getAndNext(){
		char c[N] = {};
		int p = 0;
		for (int i = pos; i < size; i++, p++, pos++){
			if (str[i] == ',' || str[i] == '\0'){
				pos++;
				return c;
			}
			c[p] = str[i];
		}
		return c;

	}

	void reset(){
		pos = 0;
	}

};