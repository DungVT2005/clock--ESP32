1.1. Lý do chọn đề tài
Trong thời đại công nghệ phát triển mạnh mẽ, các thiết bị điện tử thông minh ngày càng được ứng dụng rộng rãi trong đời sống Trong đời sống hằng ngày, việc theo dõi thời gian và quản lý các hoạt động cá nhân là rất quan trọng. Các thiết bị đồng hồ hiện đại không chỉ hiển thị thời gian mà còn tích hợp nhiều chức năng hỗ trợ như báo thức, đếm giờ, bấm giờ và theo dõi các chỉ số sức khỏe. Việc xây dựng một hệ thống đồng hồ đa năng dựa trên vi điều khiển đòi hỏi phải thiết kế cả phần cứng và phần mềm để các chức năng hoạt động chính xác và ổn định.
Bên cạnh đó, sự phát triển của các vi điều khiển như ESP32 giúp việc xây dựng các hệ thống nhúng trở nên dễ dàng và linh hoạt hơn. ESP32 có nhiều ưu điểm như hiệu năng cao, tích hợp WiFi/Bluetooth, nhiều chân GPIO và hỗ trợ nhiều giao thức giao tiếp như I2C, SPI, UART.
Xuất phát từ nhu cầu thực tế cũng như mong muốn vận dụng các kiến thức đã học về vi điều khiển, cảm biến và lập trình hệ thống nhúng, đề tài “Thiết kế và xây dựng đồng hồ đa năng” được lựa chọn. Thông qua đề tài này, một thiết bị có khả năng hiển thị thời gian và tích hợp nhiều chức năng tiện ích sẽ được xây dựng và thử nghiệm.
1.2. Mục tiêu đề tài
Mục tiêu chính của đề tài là thiết kế và xây dựng một hệ thống đồng hồ đa năng dựa trên vi điều khiển ESP32 với các chức năng cơ bản sau:
•	Hiển thị thời gian thực (giờ, phút, giây).
•	Chức năng báo thức (Alarm).
•	Chức năng đếm giờ (Timer).
•	Chức năng bấm giờ (Stopwatch).
•	Đo nhiệt độ và độ ẩm môi trường bằng cảm biến.
•	Đo nhịp tim bằng cảm biến sinh học.
•	Hiển thị dữ liệu và trạng thái hoạt động của hệ thống
•	Đưa ra cảnh báo cho người dùng.	

Ngoài ra, đề tài còn giúp:
•	Rèn luyện kỹ năng lập trình vi điều khiển.
•	Tìm hiểu cách giao tiếp với các cảm biến.
•	Xây dựng một hệ thống nhúng hoàn chỉnh từ phần cứng đến phần mềm.
1.3. Phạm vi thực hiện
•	Đề tài tập trung vào việc thiết kế và xây dựng một hệ thống đồng hồ đa năng với các chức năng cơ bản. Phạm vi thực hiện bao gồm:
•	Sử dụng ESP32 làm bộ điều khiển trung tâm.
•	Ngôn ngữ C++
•	Kết nối các cảm biến như cảm biến nhiệt độ và cảm biến nhịp tim.
•	Lập trình các chức năng hiển thị thời gian, báo thức, bấm giờ và đếm giờ.
•	Hiển thị thông tin hệ thống trên máy tính hoặc thiết bị hiển thị.
•	Đề tài chủ yếu tập trung vào mô hình thử nghiệm và nguyên lý hoạt động, chưa đi sâu vào tối ưu hóa phần cứng, thiết kế vỏ sản phẩm hoặc sản xuất thương mại.
1.4. Phương pháp thực hiện
Để thực hiện đề tài, các phương pháp sau được sử dụng:
•	Nghiên cứu tài liệu: tìm hiểu về ESP32, các cảm biến sử dụng và các giao thức giao tiếp.
•	Thiết kế hệ thống: xây dựng sơ đồ kết nối giữa vi điều khiển và các thiết 
bị ngoại vi.
•	Lập trình hệ thống: sử dụng môi trường lập trình  để viết chương trình điều khiển.
•	Sử dụng các cảm biến đo nhiệt độ , độ ẩm , nhịp tim (max30102, DHT11)
•	Thử nghiệm và kiểm tra: kiểm tra từng chức năng của hệ thống, hiệu chỉnh chương trình để đảm bảo hoạt động ổn định.
