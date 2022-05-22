# <img src="https://user-images.githubusercontent.com/99308937/169700974-bd51151e-11b6-42b8-a3dc-adf34884f652.png" alt="otter" style="zoom:100%;" />BOP!BOP! Project

> Trần Duy Khánh: 21020769
>
> K66CC-CN1-UET
>
> @duykhanhxx03

## **GIỚI THIỆU CHUNG:**

Bop!Bop! được hoàn thành dựa trên lối chơi của Google Dinosaur nhưng đã bổ sung kịch bản chơi, đồ hoạ và âm thanh sao cho sinh động hơn.

Game được viết bằng ngôn ngữ C++ và sử dụng thư viện SDL2

##### LUẬT CHƠI:

- **Nhiệm vụ của người chơi là nhập vai vào Bop - Một chú rái cá nhỏ để đưa chú vượt qua những thử thách nguy hiểm chết “rái cá” như nấm độc và các loại cây ăn thịt đáng sợ:**
  - Nhấn UP hoặc SPACE để nhảy lên
  
  - Nhấn DOWN để tiếp đất nhanh
  
  - Nhấn SPACE để chọn nhanh các lựa chọn
  
  - Nhấn ESC để Pause/ Resume
  
- **Điểm của người chơi tỉ lệ thuận với thời gian Bop sống và số Coin/ Gems thu được trong một lượt đấu:** 
  - 1 Coin/10 points
  - 1 Gems/20 points
- **Điểm càng cao thì tốc độ cũng như độ khó của game càng tăng, đòi hỏi sự tập trung cao độ của người chơi. Hãy là người ghi điểm cao nhất nhé!**

## CÁCH CÀI ĐẶT:

##### Tải xuống tại đây: [Download](https://github.com/duykhanhxx03/Bopbop/releases/tag/v1.0.1)

##### Cách kiểm tra hệ thống là x64(64bit) hay x86(32bit): [Truy cập](https://support.microsoft.com/en-us/windows/32-bit-and-64-bit-windows-frequently-asked-questions-c6ca9541-8dce-4d48-0415-94a3faa2e13d)

##### Tuỳ thuộc vào loại của hệ thống mà chọn file cài đặt một cách chính xác, trong đó:

- **Đối với hệ thống 64bit: Có thể chạy cả hai phiên bản x64(64bit) và x86(32bit)**

  - B1: Nhấp đúp vào file cài đặt (BopBopx64Setup.exe hoặc BopBopx86Setup.exe)

    ![image-20220522104420093](https://user-images.githubusercontent.com/99308937/169700614-f789bb88-3f11-45e4-9ca2-6e8af5bbc246.png)

    - Xuất hiện hộp thoại User Account Control: Bấm Yes

  - B2: Hộp thoại Setup xuất hiện

    ![image-20220522105748485](https://user-images.githubusercontent.com/99308937/169700671-94b550bc-1ba0-4fef-a034-ea8843f853d6.png)

    - Tick vào ô Creata a desktop shortcut để tạo đường dẫn ra ngoài màn hình
    - Ấn Next để đi đến trang tiếp theo

  - B3: Cài đặt

    ![image-20220522105928146](https://user-images.githubusercontent.com/99308937/169700693-84a19504-f23e-4d2c-bb06-0005e38772d2.png)

    - Nhấn Install để tiến hành cài đặt

    ![image-20220522110101587](https://user-images.githubusercontent.com/99308937/169700772-bc268c4a-94c4-4d5b-ae11-e2faa3e44d2f.png)

    - Chờ cho quá trình cài đặt được hoàn tất

    ![image-20220522110255583](https://user-images.githubusercontent.com/99308937/169700741-bb05fff9-8fdc-4cc7-b998-1ac5523f2711.png)

    - Tick Launch Bop!Bop! để chạy game ngay sau khi đóng hộp thoại
    - Nhấn Finish để hoàn tất việc cài đặt.

- **Đối với hệ thống 32bit: Chỉ chạy được phiên bản x86(32bit)**

  - Tương tự như cách cài phiên bản x64(64bit) nhưng với file cài đặt BopBopx86Setup.exe

- **Đối với Portable(Phần mềm di động):** 

  - B1: Chọn đúng phiên bản với hệ thống của mình (x64(64bit) hay x86(32bit))

  - B2: Tiến hành giải nén file ZIP bằng các phần mềm chuyên dụng như RAR hay 7Zip

  - B3: Chạy chương trình

    - Đối với phiên bản x86(32bit): BopBopx86/BopBop.exe để chạy chương trình
    - Đối với phiên bản x64(64bit): BopBopx64/BopBop.exe để chạy chương trình

- **Lưu ý: Đối với những máy có màn hình tần số quét lớn hơn 60hz thì phải lựa chọn lại thiết đặt 60hz:**

  - B1: Start -> Setting hoặc Window + I để mở hộp thoại Setting
  - B2: Setting -> System -> Display -> Advanced display settings
  - B3: Tại mục Refresh Rate, chọn 60hz


## MÔ TẢ CHỨC NĂNG

- **Có hệ thống menu đa dạng:**
  - Menu bắt đầu
  - Menu hướng dẫn chơi
  - Menu cài đặt âm thanh dạng kéo: Tăng giảm âm lượng
  - Menu tạm dừng
  - Menu khi đang chơi game: Về home, bật tắt nhanh âm thanh, nút tạm dừng/ tiếp tục
  - Menu hỏi khi thoát
  - Menu khi thua: Phân loại số sao và lời thoại
    - Nếu score<100 thì không được sao nào
    - Nếu score>=100 và score <200 thì được một sao
    - Nếu score>=200 và score<300 thì được hai sao
    - Nếu score>=300 và score<400 thì được ba sao
    - Nếu score>=400 thì vẫn được ba sao
    - Khi điểm mới là điểm cao thì sẽ in ra "HIGH SCORE", ngược lại sẽ in ra "YOUR SCORE"
- **Có hệ thống âm thanh sinh động:**
  - Nhạc nền (BGM)
  - Nhạc hiệu ứng(SFX): Nhảy, lên level, ăn vật phẩm, thua.
- **Lưu điểm cao và lưu thông tin âm thanh sau mỗi lần chơi**
- **Đếm ngược khi vừa tạm dừng để người chơi có thời gian chuẩn bị để dễ thích nghi lại với game hơn**
- **Tăng tốc sau mỗi level**

## CÁC KĨ THUẬT LẬP TRÌNH ĐƯỢC SỬ DỤNG ##

- Mảng
- Con trỏ/ Giải phóng bộ nhớ
- Class
- Đồ hoạ
- Bắt sự kiện bàn phím, chuột
- Bắt va chạm per-pixel theo từng hoạt ảnh, chuyển động
- Stringstream
- Xử lý nhảy/ rơi theo vật lý
- Cắm cờ
- Sinh số ngẫu nhiên theo điều kiện
- Chia file theo đối tượng
- Đọc, ghi file

## KẾT LUẬN

Hướng phát triển: Em sẽ cải tiến thêm các tuỳ chọn về map, nhân vật, âm thanh, bảng xếp hạng có tên người chơi trong tương lai và cũng nghiên cứu cách khắc phục sự thiếu xót của SDL như Scrolling ảnh bằng cách dùng OPENGL, SDL_Delay chỉ nhận được số mili giây nguyên (rất khó cho việc Capping FPS).

Qua thời gian làm game, em đã cải thiện được một số kĩ năng về lập trình, tìm kiếm thông tin, giải quyết vấn đề và về khả năng đọc tài liệu tiếng Anh. Không những thế, nó còn giúp em nắm chính xác cách hoạt động và cài đặt đối tượng của một chương trình, làm nền tảng vững chắc cho em sau này khi sử dụng các game engine như Unreal, Unity, Godot, Cocos2d-x .v.v.

## CREDIT

**Special thanks to:**

- Thầy Trần Quốc Long
- Thầy Nguyễn Việt Anh
- Thầy Đỗ Minh Khá

Vì đã giúp em hoàn thành bài tập này.

**Also thanks to:**

[Lazy Foo' Productions](https://lazyfoo.net/tutorials/SDL)

#### Resources:

[Oak Woods — Environment Asset 🍂 by brullov (itch.io)](https://brullov.itch.io/oak-woods)

https://rili-xl.itch.io/otter-sprite-pack

https://untiedgames.itch.io/shroom-hopper-platformer-enemy

https://www.shutterstock.com/vi/image-vector/cartoon-game-objects-2d-art-object-651295144

https://www.shutterstock.com/vi/image-vector/ui-buttons-icons-set-isolated-vector-1895144059

https://www.shutterstock.com/vi/image-vector/complete-set-graphical-user-interface-gui-247961935

https://www.shutterstock.com/vi/image-vector/seamless-land-layers-dirt-ground-landscape-1197030319

https://opengameart.org/content/goggle-eye-bee-enemy-game-character
