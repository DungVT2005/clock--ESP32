import tkinter as tk
from tkinter import ttk, scrolledtext, messagebox
import serial
import serial.tools.list_ports
import threading

class SerialMonitorApp:
    def __init__(self, root):
        self.root = root
        self.root.title("HUST - ESP32 Serial Monitor")
        self.root.geometry("700x500")
        self.root.configure(padx=10, pady=10)

        self.serial_port = None
        self.is_reading = False

        # --- KHU VỰC ĐIỀU KHIỂN (Top Frame) ---
        control_frame = tk.Frame(self.root)
        control_frame.pack(fill=tk.X, pady=(0, 10))

        tk.Label(control_frame, text="Cổng COM:", font=("Arial", 10, "bold")).pack(side=tk.LEFT)
        
        self.cb_ports = ttk.Combobox(control_frame, width=15, state="readonly")
        self.cb_ports.pack(side=tk.LEFT, padx=5)
        self.refresh_ports() # Lấy danh sách cổng COM

        tk.Button(control_frame, text="🔄 Làm mới", command=self.refresh_ports).pack(side=tk.LEFT, padx=5)

        tk.Label(control_frame, text="Baudrate:", font=("Arial", 10, "bold")).pack(side=tk.LEFT, padx=(15, 5))
        self.cb_baudrate = ttk.Combobox(control_frame, values=["9600", "115200", "256000"], width=10, state="readonly")
        self.cb_baudrate.set("115200") # Mặc định của ESP32
        self.cb_baudrate.pack(side=tk.LEFT)

        self.btn_connect = tk.Button(control_frame, text="Kết nối", bg="#2ecc71", fg="white", font=("Arial", 10, "bold"), command=self.toggle_connection)
        self.btn_connect.pack(side=tk.RIGHT, padx=5)

        # --- KHU VỰC HIỂN THỊ (Middle Frame) ---
        # Giao diện hacker: nền đen chữ xanh lá
        self.txt_console = scrolledtext.ScrolledText(self.root, bg="black", fg="#00ff00", font=("Consolas", 11), wrap=tk.WORD)
        self.txt_console.pack(fill=tk.BOTH, expand=True, pady=5)
        self.txt_console.configure(state='disabled') # Khóa chỉ đọc

        # --- KHU VỰC NHẬP LỆNH (Bottom Frame) ---
        input_frame = tk.Frame(self.root)
        input_frame.pack(fill=tk.X, pady=(10, 0))

        self.entry_cmd = tk.Entry(input_frame, font=("Consolas", 12))
        self.entry_cmd.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=(0, 5))
        self.entry_cmd.bind("<Return>", lambda event: self.send_command()) # Bấm Enter để gửi

        self.btn_send = tk.Button(input_frame, text="Gửi lệnh", bg="#3498db", fg="white", font=("Arial", 10, "bold"), command=self.send_command)
        self.btn_send.pack(side=tk.RIGHT)

    def refresh_ports(self):
        """Quét các cổng COM đang cắm vào máy tính"""
        ports = serial.tools.list_ports.comports()
        port_list = [port.device for port in ports]
        self.cb_ports['values'] = port_list
        if port_list:
            self.cb_ports.current(0)
        else:
            self.cb_ports.set("No COM found")

    def toggle_connection(self):
        """Xử lý nút Kết nối / Ngắt kết nối"""
        if not self.is_reading:
            # Thực hiện kết nối
            port = self.cb_ports.get()
            baud = self.cb_baudrate.get()
            try:
                self.serial_port = serial.Serial(port, int(baud), timeout=1)
                self.is_reading = True
                self.btn_connect.config(text="Ngắt kết nối", bg="#e74c3c")
                self.log_to_console(f"--- Đã kết nối tới {port} ở {baud} baud ---\n", color="#f1c40f")
                
                # Tạo luồng chạy ngầm để đọc dữ liệu liên tục không làm đơ giao diện
                self.read_thread = threading.Thread(target=self.read_from_serial, daemon=True)
                self.read_thread.start()
            except Exception as e:
                messagebox.showerror("Lỗi kết nối", f"Không thể mở cổng {port}.\nChi tiết: {str(e)}")
        else:
            # Thực hiện ngắt kết nối
            self.is_reading = False
            if self.serial_port and self.serial_port.is_open:
                self.serial_port.close()
            self.btn_connect.config(text="Kết nối", bg="#2ecc71")
            self.log_to_console("--- Đã ngắt kết nối ---\n", color="#e74c3c")

    def read_from_serial(self):
        """Đọc dữ liệu từ ESP32 ném lên"""
        while self.is_reading and self.serial_port.is_open:
            try:
                if self.serial_port.in_waiting > 0:
                    line = self.serial_port.readline().decode('utf-8', errors='replace')
                    self.log_to_console(line)
            except Exception:
                break

    def send_command(self):
        """Gửi lệnh từ ô nhập liệu xuống ESP32"""
        if self.serial_port and self.serial_port.is_open:
            cmd = self.entry_cmd.get()
            if cmd:
                # Gửi lệnh kèm ký tự xuống dòng (y hệt tính năng Newline của VS Code)
                self.serial_port.write((cmd + '\n').encode('utf-8'))
                self.log_to_console(f"> Gửi: {cmd}\n", color="#3498db") # In ra lệnh vừa gửi bằng màu xanh dương
                self.entry_cmd.delete(0, tk.END)
        else:
            messagebox.showwarning("Cảnh báo", "Hãy kết nối cổng COM trước khi gửi lệnh!")

    def log_to_console(self, text, color="#00ff00"):
        """In chữ lên màn hình đen"""
        self.txt_console.configure(state='normal')
        self.txt_console.insert(tk.END, text, ("color",))
        self.txt_console.tag_config("color", foreground=color)
        self.txt_console.see(tk.END) # Tự động cuộn xuống dòng mới nhất
        self.txt_console.configure(state='disabled')

if __name__ == "__main__":
    root = tk.Tk()
    app = SerialMonitorApp(root)
    root.mainloop()