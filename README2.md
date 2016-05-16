*mcfac.c*  
mcmc法によるmatrix factorizationを行うプログラム  

*matrix.c matrix.h*  
C言語で行列計算を行うためのヘッダファイル  

*matrix1.txt*  
分解する行列のデータ.最初の一行目は行数と列数,二行目以降は行列の成分を入れる.  
ただし,0は数ではなく不明値として扱う.   

*matfac2.c*  
mcfac.cで得られた行列P,Qに対してmatrix factorizationを行うプログラム  

*matrixp.txt matrixq.txt*  
matfac2.cで使う行列P,Qのデータ  

=====
*mcfac.cの実行方法*  
mcfac.cをコンパイルし実行する.途中math.hのexp関数を使っているのでgcc mcfac.c -lmでコンパイルする事に注意.  

変数  
K:	分解後の行列の大きさ.N行M列の行列はN行K列とK行M列の行列に分解される.  
STEP:	試行回数(実験では100万回行ったが10万回でも十分な結果が得られる)  
ALPHA:	数値が大きくなりすぎないように調節する係数.  
MIN MAX:	PとQの成分の最大値と最小値.  
CUT:	結果を表示する際に観測回数がCUT以下のものは表示しない.  
filename:	分割する行列のデータ.  

結果の表示  
ある一定回数以上観測されたP,Qを表示する  
回数　誤差  
P  
Q  
P*Q  
の順に表示される.  

=====
*matfac2.cの実行方法*  
mcfac.cで得られたP,Qの値をそれぞれmatrixp.txtとmatrixq.txtに代入する.  
そしてmcfac.cをコンパイルし実行する.  

変数  
K: 分解後の行列の大きさ.N行M列の行列はN行K列とK行M列の行列に分解される.  
STEP: 試行の繰り返しの回数.  
ALPHA: 勾配法のステップ幅.  
BETA: 正則化項の係数.  
filename: 読み込む行列のファイル名  
filenamep,filenameq:	P,Qが入ったデータファイル  

結果の表示
R: 分解する行列.ファイルから読み込む
P: Pの初期値.N行K列行列.ファイルから読み込む
Q: Qの初期値.K行M列行列.ファイルから読み込む

error: RとPQの差
P: 更新したN行K列行列
Q: 更新したK行M列行列
PQ: PとQを掛けた行列