import tkinter as tk
from tkinter import ttk, messagebox
import serial
import serial.tools.list_ports
import threading

class SerialMonitorApp:
    def __init__(self, root):
        self.root = root
        self.root.title("HUST - ESP32 Smart Clock Monitor")
        self.root.geometry("700x550") 
        self.root.configure(padx=10, pady=10, bg="#2c3e50")

        self.serial_port = None
        self.is_reading = False
        self.menu_buffer = []

        # --- KHU VỰC ĐIỀU KHIỂN (Top Frame) ---
        control_frame = tk.Frame(self.root, bg="#2c3e50")
        control_frame.pack(fill=tk.X, pady=(0, 10))

        tk.Label(control_frame, text="Cổng COM:", font=("Arial", 10, "bold"), bg="#2c3e50", fg="white").pack(side=tk.LEFT)
        
        self.cb_ports = ttk.Combobox(control_frame, width=15, state="readonly")
        self.cb_ports.pack(side=tk.LEFT, padx=5)
        self.refresh_ports()

        tk.Button(control_frame, text="🔄 Làm mới", command=self.refresh_ports).pack(side=tk.LEFT, padx=5)

        tk.Label(control_frame, text="Baudrate:", font=("Arial", 10, "bold"), bg="#2c3e50", fg="white").pack(side=tk.LEFT, padx=(15, 5))
        self.cb_baudrate = ttk.Combobox(control_frame, values=["9600", "115200", "256000"], width=10, state="readonly")
        self.cb_baudrate.set("115200")
        self.cb_baudrate.pack(side=tk.LEFT)

        self.btn_connect = tk.Button(control_frame, text="Kết nối", bg="#2ecc71", fg="white", font=("Arial", 10, "bold"), command=self.toggle_connection)
        self.btn_connect.pack(side=tk.RIGHT, padx=5)

        # --- KHU VỰC HIỂN THỊ ĐỒNG HỒ (Middle Frame) ---
        self.display_frame = tk.Frame(self.root, bg="black", bd=10, relief=tk.RIDGE)
        self.display_frame.pack(fill=tk.BOTH, expand=True, pady=5)

        # 1. Hiển thị Ngày tháng (Phía trên)
        self.lbl_date = tk.Label(self.display_frame, text="--/--/----", font=("Consolas", 18, "bold"), bg="black", fg="#00ffff")
        self.lbl_date.pack(pady=(15, 0))

        # 2. Hiển thị Thông số chính (Giờ / Nhiệt độ / Nhịp tim...) ở giữa
        self.lbl_time = tk.Label(self.display_frame, text="00:00:00", font=("Consolas", 75, "bold"), bg="black", fg="#00ff00")
        self.lbl_time.pack(pady=(0, 10))

        # 3. Hiển thị Menu (Ở dưới góc trái)
        self.lbl_msg = tk.Label(self.display_frame, text="System Ready...\nĐang chờ kết nối...", font=("Consolas", 12), bg="black", fg="#f1c40f", justify=tk.LEFT, anchor="nw")
        self.lbl_msg.pack(fill=tk.BOTH, expand=True, padx=20, pady=10)

        # --- KHU VỰC NHẬP LỆNH (Bottom Frame) ---
        input_frame = tk.Frame(self.root, bg="#2c3e50")
        input_frame.pack(fill=tk.X, pady=(10, 0))

        self.entry_cmd = tk.Entry(input_frame, font=("Consolas", 14))
        self.entry_cmd.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=(0, 5))
        self.entry_cmd.bind("<Return>", lambda event: self.send_command())

        self.btn_send = tk.Button(input_frame, text="Gửi lệnh", bg="#3498db", fg="white", font=("Arial", 10, "bold"), command=self.send_command)
        self.btn_send.pack(side=tk.RIGHT)

    def refresh_ports(self):
        ports = serial.tools.list_ports.comports()
        port_list = [port.device for port in ports]
        self.cb_ports['values'] = port_list
        if port_list:
            self.cb_ports.current(0)
        else:
            self.cb_ports.set("No COM found")

    def toggle_connection(self):
        if not self.is_reading:
            port = self.cb_ports.get()
            baud = self.cb_baudrate.get()
            try:
                self.serial_port = serial.Serial(port, int(baud), timeout=1)
                self.is_reading = True
                self.btn_connect.config(text="Ngắt kết nối", bg="#e74c3c")
                self.log_to_console("--- Đã kết nối ---")
                
                self.read_thread = threading.Thread(target=self.read_from_serial, daemon=True)
                self.read_thread.start()
            except Exception as e:
                messagebox.showerror("Lỗi kết nối", f"Không thể mở cổng {port}.\nChi tiết: {str(e)}")
        else:
            self.is_reading = False
            if self.serial_port and self.serial_port.is_open:
                self.serial_port.close()
            self.btn_connect.config(text="Kết nối", bg="#2ecc71")
            self.log_to_console("--- Đã ngắt kết nối ---")

    def read_from_serial(self):
        while self.is_reading and self.serial_port.is_open:
            try:
                if self.serial_port.in_waiting > 0:
                    line = self.serial_port.readline().decode('utf-8', errors='replace')
                    self.log_to_console(line)
            except Exception:
                break

    def send_command(self):
        if self.serial_port and self.serial_port.is_open:
            cmd = self.entry_cmd.get()
            if cmd:
                self.serial_port.write((cmd + '\n').encode('utf-8'))
                self.entry_cmd.delete(0, tk.END)
        else:
            messagebox.showwarning("Cảnh báo", "Hãy kết nối cổng COM trước khi gửi lệnh!")

    def log_to_console(self, text, color=None):
        self.root.after(0, self._process_serial_data, text.strip())

    # --- LOGIC XỬ LÝ HIỂN THỊ ĐÃ ĐƯỢC NÂNG CẤP ---
    def _process_serial_data(self, text):
        if not text: return

        # 1. Nhận diện các dòng thuộc Menu (Có chữ ===== hoặc format dạng "0: Clock") 
        #    và các thông báo hệ thống (Bắt đầu bằng ---)
        is_menu_item = (len(text) >= 3 and text[0].isdigit() and text[1:3] == ": ")
        
        if "=====" in text or is_menu_item or text.startswith("---"):
            # Nếu gặp tiêu đề Menu mới, xóa sạch các dòng menu cũ
            if "=====" in text:
                self.menu_buffer = []
                
            self.menu_buffer.append(text)
            
            # Chỉ giữ tối đa 8 dòng ở góc để không bị tràn
            if len(self.menu_buffer) > 8:
                self.menu_buffer.pop(0)
            self.lbl_msg.config(text="\n".join(self.menu_buffer))
            
        # 2. Nhận diện Ngày tháng (VD: Thu/25/Jun)
        elif "/" in text and len(text) <= 15:
            self.lbl_date.config(text=text)
            
        # 3. MỌI CHẾ ĐỘ CÒN LẠI (Giờ, Stopwatch, Nhiệt độ, Nhịp tim...) -> Đẩy ra giữa màn hình
        else:
            # Tự động điều chỉnh cỡ chữ:
            # - Nếu text dài (như "T: 34.3C, H: 57.1%"): Thu nhỏ chữ lại cỡ 45 để vừa khung.
            # - Nếu text ngắn (như "11:13:15"): Dùng cỡ siêu to 75.
            if len(text) > 12:
                self.lbl_time.config(font=("Consolas", 45, "bold"), text=text)
            else:
                self.lbl_time.config(font=("Consolas", 75, "bold"), text=text)

if __name__ == "__main__":
    root = tk.Tk()
    app = SerialMonitorApp(root)
    root.mainloop()