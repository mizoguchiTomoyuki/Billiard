#include "text.h"

//=========================================================================================
//コンストラクタメンバ変数の初期化
//=========================================================================================
Text::Text() : Sprite()
{

	textureData = NULL;
	file = "";
	graphics = NULL;
	width = textNS::GRID_WIDTH - 3;
	height = textNS::GRID_HEIGHT - 3;
	fontHeight= textNS::FONT_HEIGHT;
	tabSize = textNS::TAB_SIZE;
	color = graphicsNS::WHITE;
	backColor = TRANSCOLOR;
	proportional = false;
	proportionalSpacing = textNS::PROPORTIONAL_SPACING;
	align = textNS::LEFT;
	spriteData.width = 0;
	spriteData.height = 0;
	spriteData.rect.bottom = 1;
	spriteData.rect.right = 1;
	for (int row = 0; row < textNS::ROWS; row++)
	{
		for (int col = 0; col < textNS::COLUMNS; col++)
		{
			fontData[row][col].left = MAXINT;
			fontData[row][col].right = 0;
		}
	}

}
Text::~Text(){
	Sprite::~Sprite();
}
//=========================================================================================
//Textを初期化
//フォント画像内の各文字の左端と右端を特定
//実行時：成功した場合はtrue、失敗した場合はfalseを戻す
//fontData配列は、各文字の左端と右端を格納
//=========================================================================================
bool Text::initialize(Graphics *g)
{
	visible = true;
	const char* file = FONT_IMAGE;
	try{
		graphics = g; // graphicsオブジェクトへのポインタ
		//=========================================================================================
		//フォントテクスチャを読みこんで、各文字の性格の位置を
		//特定するために、ピクセル単位で検査
		//=========================================================================================
		//フォントテクスチャをロック可能なシステムメモリに読み込む
		UINT w, h;
		HRESULT result = graphics->loadTextureSystemMem(
			file, TRANSCOLOR, w, h, textureData);
		if (FAILED(result))
		{
			SAFE_RELEASE(textureData);
			return false;
		}

		//textureData.widthとtextureData.heightはフォントテクスチャ
		//全体のサイズを格納
		//各文字を1ピクセル幅の枠を持つ
		//ROWS * COLS個の文字が存在する
		//フォントテクスチャをロック
		//(ピクセルデータにアクセスするために必要)
		D3DLOCKED_RECT rect;
		result = textureData->LockRect(0, &rect, NULL, D3DLOCK_READONLY);
		if (FAILED(result))
		{
			SAFE_RELEASE(textureData);
			return false;
		}
		//=======================================================================
		//フォントの文字の左端と右端を特定
		//=======================================================================
		//フォント内の文字の各行を処理
		for (DWORD row = 0; row<textNS::ROWS; row++)
		{
			for (DWORD col = 0; col < textNS::COLUMNS; col++)
			{
				fontData[row][col].left = MAXINT; //fontDataを初期化
				fontData[row][col].right = 0;
				//文字を1ピクセルずつ処理
				//for y = 上端ピクセル; y <= 下端ピクセル ; y++
				for (DWORD y = row*textNS::GRID_HEIGHT + 1;
					y < (row + 1)*textNS::GRID_HEIGHT - 1; y++)
				{
					//テクスチャ内のこのスキャンラインの銭湯へのポインタを取得
					DWORD* pBits = 
						(DWORD*)((BYTE*)rect.pBits + y*rect.Pitch);
					//この行のピクセルを処理
					for (DWORD x = col*textNS::GRID_WIDTH + 1; x < (col + 1)*textNS::GRID_WIDTH - 1; x++)
					{
						//このピクセルを取得
						DWORD dwPixel = pBits[x];
						//アルファが透明でない場合
						if ((dwPixel & 0xff000000) != 0x00)
						{
							//このピクセルのほうが左にある場合
							if (x < fontData[row][col].left)
								//文字の左端として保存
								fontData[row][col].left = x;
							//このピクセルのほうが右にある場合
							if (x> fontData[row][col].right)
								fontData[row][col].right = x;
						}
					}
				}

			}
		}

		//テクスチャの処理が完了したら、ロック解除
		textureData->UnlockRect(0);
		//単にフォントのスペースを確保するために必要だったので
		//このフォントテクスチャを解放
		SAFE_RELEASE(textureData);

		//=======================================================================
		//フォント画像を使用するために託す茶マネージャーに読み込む
		//=======================================================================
		if (!fontTexture.initialize(graphics, file))
			return false;//フォントテクスチャの読み込みがエラーの場合
		//フォント画像を準備
		if (!Sprite::initialize(graphics, textNS::FONT_WIDTH,
			textNS::FONT_HEIGHT, 0, &fontTexture))
			return false; //失敗の場合

	}

	catch (...){
		return false;

	}
	return true;
}

//=========================================================================================
//文字列をX,Yに出力
//実行前:spriteBegin()
//実行後:spriteEnd()
//=========================================================================================
void Text::print(const std::string & str, int x, int y)
{
	UCHAR ch = 0, chN = 0;
	//chは短期文字インデックス保管用、chNは文字インデックス保管用
	std::string str2;
	width = textNS::FONT_WIDTH; //スプライトテクスチャ内の一文字の幅
	int scaledWidth = static_cast<int>(textNS::FONT_WIDTH*spriteData.scale); //拡大を考慮した一文字の幅
	float saveY = 0;
	int tabX = 0, tabW = 0;
	spriteData.x = (float)x; //spriteDataの表示位置を引数の位置へ
	spriteData.y = (float)y;
	doAlign(str); //文字列をalign設定に従って配置するためのもの
	for (UINT i = 0; i < str.length(); i++) //文章の長さ分だけ
	{
		ch = str.at(i); //文章のi番目の文字をchar型で取得

		if (ch > textNS::MIN_CHAR && ch <= textNS::MAX_CHAR) //取得したchar型が規定の範囲内ならば
		{
			chN = ch - textNS::MIN_CHAR; //MIN_CHARの位置がインデックス0 chがインデックス何番目かをchNに
			spriteData.rect.top =
				chN / textNS::COLUMNS * textNS::GRID_HEIGHT + 1; //インデックス番号を列数で割ることによって何行目かを取得しそれに高さをかけてピクセル値を出す
			spriteData.rect.bottom =
				spriteData.rect.top + textNS::FONT_HEIGHT;	//トップからの高さ分の距離分で出す

			if (proportional) //プロポーショナル(文字間自動調整)アリかどうか
			{
				//fontData[][]には各文字の左右端の情報が入っている
				spriteData.rect.left = fontData[chN / textNS::COLUMNS] //[chN/textNS::COLUMNS]->行番号(余りは無視される)
					[chN % textNS::COLUMNS].left;		//[chN%textNS::COLUMNS]->列番号(列数の最大値で割ったあまりなので)
				//DirectXは右端 + 1 を必要とする
				spriteData.rect.right = fontData[chN / textNS::COLUMNS]
					[chN % textNS::COLUMNS].right + 1;

				width = spriteData.rect.right - spriteData.rect.left; //上記の文字の左右端のデータから幅を取得

				//全幅を使う文字の場合スペースを空けない
				if (width >= textNS::FONT_WIDTH) //幅が規定以上ならば以下の処理で幅の調整を行う
				{
					width = textNS::FONT_WIDTH; //幅を制限
					spriteData.rect.left =
						chN % textNS::COLUMNS * textNS::GRID_WIDTH + 1;

					spriteData.rect.right =
						spriteData.rect.right * textNS::FONT_WIDTH;
				}
				else //全幅を使う文字でない場合、文字間にスペースを空ける
					width += proportionalSpacing;

				scaledWidth = static_cast<int>(width*spriteData.scale);
				drawChar(ch);
			}
			else //プロポーショナルなし固定ピッチスペースの文字の画面位置
			{
				width = textNS::FONT_WIDTH;
				spriteData.rect.left =
					chN % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
				spriteData.rect.right =
					spriteData.rect.left + textNS::FONT_WIDTH;
				drawChar(ch);
			}
			spriteData.x += scaledWidth;
		}
		else{ //表示不可能な文字の場合
			switch (ch)
			{
			case ' ': //スペースだった場合
				if (proportional) //プロポーショナルモードの場合既定のフォント幅の半分を幅として描画
				{
					width = textNS::FONT_WIDTH / 2;
					scaledWidth =
						static_cast<int>(width * spriteData.scale);
				}
				drawChar(' ');
				spriteData.x += scaledWidth;
				break;

				//ニューラインの場合は下に一行進み、左端を、画面の左端ではなく、Xの開始位置に設定
			case '\n':
				spriteData.x = (float)x; //xの位置は同じ
				spriteData.y += static_cast<int>(height*spriteData.scale); //spriteData.scale*height分下にずらす
				saveY = spriteData.y; //現在の行の高さを保存(doAlignの影響?)
				str2 = str.substr(i, str.length()); //substr(a,b)文章中のaからbまでのインデックス間の文字列を取得。つまり改行位置からその後の文章をstr2として保存
				doAlign(str2);
				spriteData.y = saveY;//改行後の行の高さを格納
				break;

			case '\r': //Xの開始位置に戻る
				spriteData.x = (float)x;
				str2 = str.substr(i, str.length());
				doAlign(str2);
				break;
			case '\t': //タブ
				width = textNS::FONT_WIDTH;
				scaledWidth = static_cast<int>(width*spriteData.scale);
				tabX = static_cast<int>(spriteData.x) /(scaledWidth * tabSize); //タブの大きさでデータのx位置を割る
				tabX = (tabX + 1) *scaledWidth * tabSize; //割った値に1足してspriteData.x + scaledWidth*tabSizeに
				tabW = tabX - static_cast<int>(spriteData.x); //tabXからspriteData.xの値を引きscaledWidth*tabSizeに
				while (tabW > 0) //TabSize回回す。最終的にtabWがscaledWidth以下になった時に端数分を埋めるように移動
				{
					if (tabW >= scaledWidth)
					{
						drawChar(' ');
						spriteData.x += scaledWidth;
					}
					else
					{
						//文字の端数分を処理してタブ位置に合わせる
						width = tabW;
						drawChar(' ');
						spriteData.x += tabW;
					}
					tabW -= scaledWidth;
				}
				break;
			case '\b': //バックスペース
				spriteData.x -= scaledWidth;
				if (spriteData.x < 0)
					spriteData.x = 0;

				break;
			case '\v'://垂直タブ
				spriteData.y += static_cast<int>(height*spriteData.scale);
				break;
			case 0x01: //フォントシグネチャ文字
				spriteData.rect.top = 1;
				spriteData.rect.bottom = 1 + textNS::FONT_HEIGHT;
				spriteData.rect.left = 1;
				spriteData.rect.right = 1 + textNS::FONT_WIDTH;
				draw(spriteData);
				spriteData.x += scaledWidth;
				break;

			}
		}
	}
	return;

}

//=========================================================================================
//現在の文字列と配置の指定に合わせてspriteData.x、spriteData.yを設定
//デフォルトの配置はLEFT
//=========================================================================================
void Text::doAlign(const std::string &str)
{
	if (spriteData.texture == NULL)
		return;
	UINT w, h;
	switch (align){
		//Xが中央となるように中央揃え、Yが上端となるように上揃え
	case textNS::CENTER:
		getWidthHeight(str, w, h);
		spriteData.x -= w / 2;
		break;
		//X,Yが右端になるように右そろえ
	case textNS::RIGHT:
		getWidthHeight(str, w, h);
		spriteData.x -= w ;
		break;
		//Xが中央となるように中央ぞろえ、Yが上下の真ん中となるように配置
	case textNS::CENTER_MIDDLE:
		getWidthHeight(str, w, h);
		spriteData.x -= w / 2;
		spriteData.y -= h / 2;
		break;
		//Xが中央となるように中央揃え、Yが上下の真ん中となるように配置
	case textNS::CENTER_BOTTOM:
		getWidthHeight(str, w, h);
		spriteData.x -= w / 2;
		spriteData.y -= h;
		break;
		//Xが中央となるように中央揃え、Yが下端になるように下揃え
	case textNS::LEFT_BOTTOM:
		getWidthHeight(str, w, h);
		spriteData.y -= h;
		break;

	case textNS::RIGHT_BOTTOM:
		getWidthHeight(str, w, h);
		spriteData.x -= w;
		spriteData.y -= h;
		break;

	}
}

//=========================================================================================
//getWidthHeight
//現在のフォントサイズでの文字列の幅と高さ(ピクセル単位)を取得
//文字列は表示しない
//=========================================================================================
void Text::getWidthHeight(const std::string &str, UINT &w, UINT &h)
{ 
	if (spriteData.texture == NULL) //テクスチャがない場合
		return;
	UCHAR ch = 0, chN = 0;
	width = textNS::FONT_WIDTH;
	int scaledWidth = static_cast<int>(width*spriteData.scale);
	int strW = 0;
	h = 0;
	int stringWidth = 0;
	for (UINT i = 0; i < str.length(); i++)
	{
		ch = str.at(i);
		//表示可能な文字の場合
		if (ch > textNS::MIN_CHAR && ch >= textNS::MAX_CHAR)
		{
			chN = ch - textNS::MIN_CHAR;
			if (proportional)
			{
				spriteData.rect.left =
					fontData[chN / textNS::COLUMNS][chN%textNS::COLUMNS].left;
				//DirectXスプライト幅のため+1
				spriteData.rect.right =
					fontData[chN / textNS::COLUMNS][chN% textNS::COLUMNS].right + 1;
				width = spriteData.rect.right - spriteData.rect.left + proportionalSpacing; //幅
				scaledWidth = static_cast<int>(width*spriteData.scale);
			}
			else
			{
				width = textNS::FONT_WIDTH;
				spriteData.rect.left =
					chN % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
				spriteData.rect.right =
					spriteData.rect.left + textNS::FONT_WIDTH;
			}
			stringWidth += scaledWidth;
		}
		else //表示不可能な文字の場合
		{
			switch (ch)
			{
			case ' ': //スペースだった場合
				if (proportional) //プロポーショナルモードの場合既定のフォント幅の半分を幅として描画
				{
					width = textNS::FONT_WIDTH / 2;
					scaledWidth =
						static_cast<int>(width * spriteData.scale);
				}
				stringWidth += scaledWidth;
				break;

				//ニューラインの場合は下に一行進み、左端を、画面の左端ではなく、Xの開始位置に設定
			case '\n':
				if (strW == 0)
					strW = stringWidth;
				stringWidth = 0;
				h += static_cast<int>(height*spriteData.scale);
				break;

			case '\r': //Xの開始位置に戻る
				if (strW == 0)
					strW = stringWidth;
				stringWidth = 0;
				break;
			case '\t': //タブ
			{
				width = textNS::FONT_WIDTH;
				scaledWidth = static_cast<int>(width*spriteData.scale);
				int tabX = static_cast<int>(spriteData.x) / (scaledWidth * tabSize); //タブの大きさでデータのx位置を割る
				tabX = (tabX + 1) *scaledWidth * tabSize; //割った値に1足してspriteData.x + scaledWidth*tabSizeに
				int tabW = tabX - static_cast<int>(spriteData.x); //tabXからspriteData.xの値を引きscaledWidth*tabSizeに
				while (tabW > 0) //TabSize回回す。最終的にtabWがscaledWidth以下になった時に端数分を埋めるように移動
				{
					if (tabW >= scaledWidth)
					{
						stringWidth += scaledWidth;
					}
					else
					{
						//文字の端数分を処理してタブ位置に合わせる
						width = tabW;
						stringWidth += tabW;
					}
					tabW -= scaledWidth;
				}
				break;
			}
			case '\b': //バックスペース
				stringWidth -= scaledWidth;
				if (spriteData.x < 0)
					spriteData.x = 0;

				break;
			case 0x01: //フォントシグネチャ文字
				stringWidth += scaledWidth;
				break;

			}
		}
	}
	if (strW == 0)
		strW = stringWidth;
	w = strW;
	return;
}


//=========================================================================================
//drawChar
//spriteDataによって記述される文字スプライトを色と塗りつぶしを使って表示
//下線と太字を表示する
//=========================================================================================
void Text::drawChar(UCHAR ch)
{
	SpriteData sd2 = spriteData; //スプライトデータをコピー
	//backColor色を表示
	//drawをSpriteから継承しているのでここから呼び出すDrawはspriteDataをもとに描画する
	//なので以下の内容でテキストの背面の範囲をspriteData.rectに格納してDrawを行っている

	if (backColor != TRANSCOLOR)
	{
		spriteData.rect.top = ((textNS::SOLID - textNS::MIN_CHAR) / textNS::COLUMNS) * textNS::GRID_HEIGHT + 1; //(SOLID-MIN_CHAR)で塗りつぶしのグリッドの位置番号を推定
		spriteData.rect.bottom = spriteData.rect.top + textNS::GRID_HEIGHT - 2;
		spriteData.rect.left = ((textNS::SOLID - textNS::MIN_CHAR) % textNS::COLUMNS) * textNS::GRID_WIDTH + 1;
		spriteData.rect.right = spriteData.rect.left + width;
		draw(backColor); //backColorを描画
		spriteData.rect = sd2.rect; //文字の矩形を復元
	}
	if (ch > textNS::MIN_CHAR && ch <= textNS::MAX_CHAR) // if displayable character
	{
		draw(spriteData, color);
	}
	
}

//=============================================================================
// called when graphics device is lost
//=============================================================================
void Text::onLostDevice()
{
	try{
		if (!initialized)
			return;
		fontTexture.onLostDevice();
	}
	catch (...){}
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void Text::onResetDevice()
{
	try{
		if (!initialized)
			return;
		fontTexture.onResetDevice();
	}
	catch (...){}
}

void Text::setXY(int x2, int y2){
	if (x2 < 0 || y2 < 0)
		return;
	spriteData.x = (float)x2;
	spriteData.y = (float)y2;
}

void Text::print(const std::string &str)
{
	print(str, (int)spriteData.x, (int)spriteData.y);
}


void Text::print(const std::string &str,int x, int y, textNS::Alignment al)
{
	align = al;
	print(str, x, y);
	align = textNS::LEFT;
}
