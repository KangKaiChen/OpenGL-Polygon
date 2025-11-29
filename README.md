# OpenGL-Polygon
繪製多邊形功能,並且完成基本要求功能,在最後要將最後頂點與最初頂點連線
一個互動式的 2D 多邊形編輯器 (在 PolygonEdit 視窗中，讓使用者透過點擊、拖曳來創建和修改多邊形輪廓) 和 一個動態的 2D 多邊形生成與動畫系統 (在 GeneratePolygons 視窗中，隨機生成彩色多邊形並使其在視窗邊界內彈跳移動)。

作業環境:win11 64bit
編譯器:Visual Studio 2022
3.(作業指定功能)

(1)繪製一個多邊形
-已完成滑鼠點擊各頂點位置並顯示
-已完成依點擊順序將頂點連線
-已完成設定一個按鈕,能將最後的頂點與最初的頂點連線
-已完成選取要刪除的點並設定一個按鈕即可刪除
-已完成將頂點拖曳使多邊形能夠變形
(2)自動產生純色多邊形
-已完成設定一個按鈕,可以自動產生多邊形
-已完成讓產生的多邊形可以持續移動並且碰掉視窗邊界可以反彈
-已完成在自動產生多邊形時可以隨機產生多邊形不同邊數
-已完成在自動產生多邊形時可以隨機產生多邊形不同顏色
(3)已完成設定按鈕可在上述兩種模式間做變換
(4)已完成將椒類圖片放到介面上
(5)已完成設定一個按鈕可實現exit功能


4.(作業操作)
繪製多邊形(polygonEdit):介面左邊初始畫面為繪製多邊形功能,並且完成基本要求功能,在最後要將最後頂點與最初頂點連線需按鍵盤"c"鍵連線
				若要將欲選取的頂點給刪除需先點選想要的頂點(選中頂點會將頂點顏色更改至黃色),並且按下鍵盤"d"鍵即可刪除
產生多邊形(GeneratePolygon):設定一個按鈕(GeneratePolygon)可切換至產生多邊形狀態,按下鍵盤"c"可以產生隨機邊數,隨機顏色,隨機大小多
				    邊形,若想更改方塊中顏色R值可按鍵盤"r"鍵可以增加多邊形R值,按下鍵盤"e"鍵可以降低多邊形R值,更改方塊中
				    顏色G值可按鍵盤"g"鍵可以增加多邊形G值,按下鍵盤"f"鍵可以降低多邊形G值,更改方塊中顏色B值可按鍵盤"b"鍵

#5.(參考網址)

https://www.getit01.com/p201806193178220/ (按鍵相關功能參考網址)
https://edisonx.pixnet.net/blog/post/35196668 (產生多邊形功能中亂數產生值的公式)
https://wizardforcel.gitbooks.io/fltk-manual/content/6.html (FLTK中關於鍵盤事件處理的相關資訊)
https://www.fltk.org/doc-1.3/basics.html (FLTK相關資訊)
https://ppfocus.com/0/cu85d5498.html ,https://www.shuxuele.com/algebra/circle-equations.html (產生圓形參考並且轉化為各種多邊形)
https://blog.csdn.net/kuokuo666/article/details/88980648 (點與點之間畫線參考)
http://axdc11239.blogspot.com/2012/03/opengl.html (opengl相關功能 ex.畫線,拖曳)
https://gist.github.com/mengting7tw/9c06d04aaedf44340edc87a0e2f6730c (滑鼠拖曳參考)

6.(bonus)
於鍵盤設定按鍵可以更改多邊形顏色中R,G,B值 ,更改R值("r"可增加R值,"e"可降低R值), 更改G值("g"可增加G值,"f"可降低G值),更改B值("b"可增加
B值,"v"可降低B值)
於鍵盤設定按鍵可刪除最後一個產生的的多邊形(按鍵"d"鍵可刪除多邊形)

<img width="1253" height="804" alt="image" src="https://github.com/user-attachments/assets/be35eb52-cc44-4cb2-be91-6920b45ed65f" />
