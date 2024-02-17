module H200_CPLD_Motor12(
input[7:0] addr,
inout[7:0] data,
input reset_n,
input write_n,
input read_n,
input select_n,
output int0,
output int1,
output int2,
output int3,
input fsmcClk,
input mainClk,
input[7:0] nopA,
input[5:0] nopB,
output[7:0] cdcA,
output[7:0] cdcB,
output[7:0] cdcC,
output[7:0] cdcD,
output[3:0] cdcE,
output stepMotor1En,
output stepMotor1Dir,
output stemMotor1PWM,
output stepMotor2En,
output stepMotor2Dir,
output stemMotor2PWM,
output stepMotor3En,
output stepMotor3Dir,
output stemMotor3PWM,
output stepMotor4En,
output stepMotor4Dir,
output stemMotor4PWM,
output stepMotor5En,
output stepMotor5Dir,
output stemMotor5PWM,
output stepMotor6En,
output stepMotor6Dir,
output stemMotor6PWM,
output stepMotor7En,
output stepMotor7Dir,
output stemMotor7PWM,
output stepMotor8En,
output stepMotor8Dir,
output stemMotor8PWM,
output stepMotor9En,
output stepMotor9Dir,
output stemMotor9PWM,
output stepMotor10En,
output stepMotor10Dir,
output stemMotor10PWM,
output stepMotor11En,
output stepMotor11Dir,
output stemMotor11PWM,
output stepMotor12En,
output stepMotor12Dir,
output stemMotor12PWM
);

//电机中断线
wire stepMotorInt1;
wire stepMotorInt2;
wire stepMotorInt3;
wire stepMotorInt4;
wire stepMotorInt5;
wire stepMotorInt6;
wire stepMotorInt7;
wire stepMotorInt8;
wire stepMotorInt9;
wire stepMotorInt10;
wire stepMotorInt11;
wire stepMotorInt12;

//电机控制与脉冲计数线
wire [15:0]stepMotorControlLine1;
wire [7:0]stepMotorDataLine1;

wire [15:0]stepMotorControlLine2;
wire [7:0]stepMotorDataLine2;

wire [15:0]stepMotorControlLine3;
wire [7:0]stepMotorDataLine3;

wire [15:0]stepMotorControlLine4;
wire [7:0]stepMotorDataLine4;

wire [15:0]stepMotorControlLine5;
wire [7:0]stepMotorDataLine5;

wire [15:0]stepMotorControlLine6;
wire [7:0]stepMotorDataLine6;

wire [15:0]stepMotorControlLine7;
wire [7:0]stepMotorDataLine7;

wire [15:0]stepMotorControlLine8;
wire [7:0]stepMotorDataLine8;

wire [15:0]stepMotorControlLine9;
wire [7:0]stepMotorDataLine9;

wire [15:0]stepMotorControlLine10;
wire [7:0]stepMotorDataLine10;

wire [15:0]stepMotorControlLine11;
wire [7:0]stepMotorDataLine11;

wire [15:0]stepMotorControlLine12;
wire [7:0]stepMotorDataLine12;
	

//步进电机模块
StepMotorWithDAcc stepMotor1(
.rst_n(reset_n),
.clk(mainClk),
.register(stepMotorControlLine1),
.pul_num(stepMotorDataLine1),
.en(stepMotor1En),
.cw(stepMotor1Dir),
.pul(stemMotor1PWM),
.int_p(stepMotorInt1)
);

//步进电机模块
StepMotorWithDAcc stepMotor2(
.rst_n(reset_n),
.clk(mainClk),
.register(stepMotorControlLine2),
.pul_num(stepMotorDataLine2),
.en(stepMotor2En),
.cw(stepMotor2Dir),
.pul(stemMotor2PWM),
.int_p(stepMotorInt2)
);

//步进电机模块
StepMotorWithDAcc stepMotor3(
.rst_n(reset_n),
.clk(mainClk),
.register(stepMotorControlLine3),
.pul_num(stepMotorDataLine3),
.en(stepMotor3En),
.cw(stepMotor3Dir),
.pul(stemMotor3PWM),
.int_p(stepMotorInt3)
);

//步进电机模块
StepMotorWithDAcc stepMotor4(
.rst_n(reset_n),
.clk(mainClk),
.register(stepMotorControlLine4),
.pul_num(stepMotorDataLine4),
.en(stepMotor4En),
.cw(stepMotor4Dir),
.pul(stemMotor4PWM),
.int_p(stepMotorInt4)
);

//步进电机模块
StepMotorWithDAcc stepMotor5(
.rst_n(reset_n),
.clk(mainClk),
.register(stepMotorControlLine5),
.pul_num(stepMotorDataLine5),
.en(stepMotor5En),
.cw(stepMotor5Dir),
.pul(stemMotor5PWM),
.int_p(stepMotorInt5)
);

//步进电机模块
StepMotorWithDAcc stepMotor6(
.rst_n(reset_n),
.clk(mainClk),
.register(stepMotorControlLine6),
.pul_num(stepMotorDataLine6),
.en(stepMotor6En),
.cw(stepMotor6Dir),
.pul(stemMotor6PWM),
.int_p(stepMotorInt6)
);

//步进电机模块
StepMotorWithDAcc stepMotor7(
.rst_n(reset_n),
.clk(mainClk),
.register(stepMotorControlLine7),
.pul_num(stepMotorDataLine7),
.en(stepMotor7En),
.cw(stepMotor7Dir),
.pul(stemMotor7PWM),
.int_p(stepMotorInt7)
);

//步进电机模块
StepMotorWithDAcc stepMotor8(
.rst_n(reset_n),
.clk(mainClk),
.register(stepMotorControlLine8),
.pul_num(stepMotorDataLine8),
.en(stepMotor8En),
.cw(stepMotor8Dir),
.pul(stemMotor8PWM),
.int_p(stepMotorInt8)
);

//步进电机模块
StepMotorWithDAcc stepMotor9(
.rst_n(reset_n),
.clk(mainClk),
.register(stepMotorControlLine9),
.pul_num(stepMotorDataLine9),
.en(stepMotor9En),
.cw(stepMotor9Dir),
.pul(stemMotor9PWM),
.int_p(stepMotorInt9)
);

//步进电机模块
StepMotorWithDAcc stepMotor10(
.rst_n(reset_n),
.clk(mainClk),
.register(stepMotorControlLine10),
.pul_num(stepMotorDataLine10),
.en(stepMotor10En),
.cw(stepMotor10Dir),
.pul(stemMotor10PWM),
.int_p(stepMotorInt10)
);

//步进电机模块
StepMotorWithDAcc stepMotor11(
.rst_n(reset_n),
.clk(mainClk),
.register(stepMotorControlLine11),
.pul_num(stepMotorDataLine11),
.en(stepMotor11En),
.cw(stepMotor11Dir),
.pul(stemMotor11PWM),
.int_p(stepMotorInt11)
);

//步进电机模块
StepMotorWithDAcc stepMotor12(
.rst_n(reset_n),
.clk(mainClk),
.register(stepMotorControlLine12),
.pul_num(stepMotorDataLine12),
.en(stepMotor12En),
.cw(stepMotor12Dir),
.pul(stemMotor12PWM),
.int_p(stepMotorInt12)
);

//实时中断更新模块
reg [11:0]interrupt;
wire [11:0]interrupt_clear;


always@(posedge stepMotorInt1 or posedge interrupt_clear[0])
begin
	if(interrupt_clear[0])
		interrupt[0] <= 1'b0 ;
	else
		interrupt[0] <= 1'b1 ;
end
always@(posedge stepMotorInt2 or posedge interrupt_clear[1])
begin
	if(interrupt_clear[1])
		interrupt[1] <= 1'b0 ;
	else
		interrupt[1] <= 1'b1 ;
end
always@(posedge stepMotorInt3 or posedge interrupt_clear[2])
begin
	if(interrupt_clear[2])
		interrupt[2] <= 1'b0 ;
	else
		interrupt[2] <= 1'b1 ;
end
always@(posedge stepMotorInt4 or posedge interrupt_clear[3])
begin
	if(interrupt_clear[3])
		interrupt[3] <= 1'b0 ;
	else
		interrupt[3] <= 1'b1 ;
end
always@(posedge stepMotorInt5 or posedge interrupt_clear[4])
begin
	if(interrupt_clear[4])
		interrupt[4] <= 1'b0 ;
	else
		interrupt[4] <= 1'b1 ;
end
always@(posedge stepMotorInt6 or posedge interrupt_clear[5])
begin
	if(interrupt_clear[5])
		interrupt[5] <= 1'b0 ;
	else
		interrupt[5] <= 1'b1 ;
end
always@(posedge stepMotorInt7 or posedge interrupt_clear[6])
begin
	if(interrupt_clear[6])
		interrupt[6] <= 1'b0 ;
	else
		interrupt[6] <= 1'b1 ;
end
always@(posedge stepMotorInt8 or posedge interrupt_clear[7])
begin
	if(interrupt_clear[7])
		interrupt[7] <= 1'b0 ;
	else
		interrupt[7] <= 1'b1 ;
end
always@(posedge stepMotorInt9 or posedge interrupt_clear[8])
begin
	if(interrupt_clear[8])
		interrupt[8] <= 1'b0 ;
	else
		interrupt[8] <= 1'b1 ;
end
always@(posedge stepMotorInt10 or posedge interrupt_clear[9])
begin
	if(interrupt_clear[9])
		interrupt[9] <= 1'b0 ;
	else
		interrupt[9] <= 1'b1 ;
end
always@(posedge stepMotorInt11 or posedge interrupt_clear[10])
begin
	if(interrupt_clear[10])
		interrupt[10] <= 1'b0 ;
	else
		interrupt[10] <= 1'b1 ;
end
always@(posedge stepMotorInt12 or posedge interrupt_clear[11])
begin
	if(interrupt_clear[11])
		interrupt[11] <= 1'b0 ;
	else
		interrupt[11] <= 1'b1 ;
end

//将中断讯息实时输出
assign int1 = (interrupt == 12'b0);


FSMC_Interface FSMC_InterfaceImplement(
.addr(addr),
.data(data),
.reset_n(reset_n),
.write_n(write_n),
.read_n(read_n),
.select_n(select_n),
.mainClk(mainClk),
.cdcA(cdcA),
.cdcB(cdcB),
.cdcC(cdcC),
.cdcD(cdcD),
.cdcE(cdcE),
.nopA(nopA),
.nopB(nopB),
.stepMotorControlLine1(stepMotorControlLine1),
.stepMotorDataLine1(stepMotorDataLine1),
.stepMotorControlLine2(stepMotorControlLine2),
.stepMotorDataLine2(stepMotorDataLine2),
.stepMotorControlLine3(stepMotorControlLine3),
.stepMotorDataLine3(stepMotorDataLine3),
.stepMotorControlLine4(stepMotorControlLine4),
.stepMotorDataLine4(stepMotorDataLine4),
.stepMotorControlLine5(stepMotorControlLine5),
.stepMotorDataLine5(stepMotorDataLine5),
.stepMotorControlLine6(stepMotorControlLine6),
.stepMotorDataLine6(stepMotorDataLine6),
.stepMotorControlLine7(stepMotorControlLine7),
.stepMotorDataLine7(stepMotorDataLine7),
.stepMotorControlLine8(stepMotorControlLine8),
.stepMotorDataLine8(stepMotorDataLine8),
.stepMotorControlLine9(stepMotorControlLine9),
.stepMotorDataLine9(stepMotorDataLine9),
.stepMotorControlLine10(stepMotorControlLine10),
.stepMotorDataLine10(stepMotorDataLine10),
.stepMotorControlLine11(stepMotorControlLine11),
.stepMotorDataLine11(stepMotorDataLine11),
.stepMotorControlLine12(stepMotorControlLine12),
.stepMotorDataLine12(stepMotorDataLine12),
.interrupt_clear(interrupt_clear),
.interrupt(interrupt)
);





endmodule


//FSMC接口
module FSMC_Interface(
input[7:0] addr,
inout[7:0] data,
input reset_n,
input write_n,
input read_n,
input select_n,
input mainClk,
output reg [7:0]cdcA,
output reg [7:0]cdcB,
output reg [7:0]cdcC,
output reg [7:0]cdcD,
output reg [3:0]cdcE,
input[7:0] nopA,
input[5:0] nopB,
//电机相关接口
output reg [15:0]stepMotorControlLine1,
output reg [7:0]stepMotorDataLine1,
output reg [15:0]stepMotorControlLine2,
output reg [7:0]stepMotorDataLine2,
output reg [15:0]stepMotorControlLine3,
output reg [7:0]stepMotorDataLine3,
output reg [15:0]stepMotorControlLine4,
output reg [7:0]stepMotorDataLine4,
output reg [15:0]stepMotorControlLine5,
output reg [7:0]stepMotorDataLine5,
output reg [15:0]stepMotorControlLine6,
output reg [7:0]stepMotorDataLine6,
output reg [15:0]stepMotorControlLine7,
output reg [7:0]stepMotorDataLine7,
output reg [15:0]stepMotorControlLine8,
output reg [7:0]stepMotorDataLine8,
output reg [15:0]stepMotorControlLine9,
output reg [7:0]stepMotorDataLine9,
output reg [15:0]stepMotorControlLine10,
output reg [7:0]stepMotorDataLine10,
output reg [15:0]stepMotorControlLine11,
output reg [7:0]stepMotorDataLine11,
output reg [15:0]stepMotorControlLine12,
output reg [7:0]stepMotorDataLine12,
//中断相关接口
output [11:0]interrupt_clear,
input [11:0]interrupt
);
wire rd;
wire wr;
assign rd = (select_n | read_n);
assign wr = (select_n | write_n);
reg [7:0]dataOutReg;
assign data = (!rd) ? dataOutReg : 8'hzz;


reg [7:0]write_buf;
//清除中断
assign interrupt_clear[7:0] = ((!wr)&&(addr[7:0] == 8'd0))?data:8'd0;
assign interrupt_clear[11:8] = ((!wr)&&(addr[7:0] == 8'd1))?data[3:0]:4'd0;

always @(negedge mainClk or negedge reset_n)
begin
	if(!reset_n)
	begin
			cdcA <=  8'hff;
			cdcB <=  8'hff;
			cdcC <=  8'hff;
			cdcD <=  8'hff;
			cdcE <=  4'hf;
			stepMotorControlLine1 <= 16'd0;
			stepMotorDataLine1 <= 8'd0;
			stepMotorControlLine2 <= 16'd0;
			stepMotorDataLine2 <= 8'd0;
			stepMotorControlLine3 <= 16'd0;
			stepMotorDataLine3 <= 8'd0;
			stepMotorControlLine4 <= 16'd0;
			stepMotorDataLine4 <= 8'd0;
			stepMotorControlLine5 <= 16'd0;
			stepMotorDataLine5 <= 8'd0;
			stepMotorControlLine6 <= 16'd0;
			stepMotorDataLine6 <= 8'd0;
			stepMotorControlLine7 <= 16'd0;
			stepMotorDataLine7 <= 8'd0;
			stepMotorControlLine8 <= 16'd0;
			stepMotorDataLine8 <= 8'd0;
			stepMotorControlLine9 <= 16'd0;
			stepMotorDataLine9 <= 8'd0;
			stepMotorControlLine10 <= 16'd0;
			stepMotorDataLine10 <= 8'd0;
			stepMotorControlLine11 <= 16'd0;
			stepMotorDataLine11 <= 8'd0;
			stepMotorControlLine12 <= 16'd0;
			stepMotorDataLine12 <= 8'd0;
	end
	else if(!wr)
	begin
		case(addr)
		8'd2:
		begin
			write_buf[7:0] <= data[7:0];	
		end
		8'd3:
		begin
			stepMotorControlLine1[7:0] <= write_buf[7:0];
			stepMotorControlLine1[15:8] <=  data[7:0];
		end
		8'd4:
		begin
			stepMotorDataLine1[7:0] = data[7:0];
		end
		8'd5:
		begin
			stepMotorControlLine2[7:0] <= write_buf[7:0];
			stepMotorControlLine2[15:8] <=  data[7:0];
		end
		8'd6:
		begin
			stepMotorDataLine2[7:0] = data[7:0];
		end
		8'd7:
		begin
			stepMotorControlLine3[7:0] <= write_buf[7:0];
			stepMotorControlLine3[15:8] <=  data[7:0];	
		end
		8'd8:
		begin
			stepMotorDataLine3[7:0] = data[7:0];
		end
		8'd9:
		begin
			stepMotorControlLine4[7:0] <= write_buf[7:0];
			stepMotorControlLine4[15:8] <=  data[7:0];
		end
		8'd10:
		begin
			stepMotorDataLine4[7:0] = data[7:0];	
		end
		8'd11:
		begin
			stepMotorControlLine5[7:0] <= write_buf[7:0];
			stepMotorControlLine5[15:8] <=  data[7:0];
		end
		8'd12:
		begin
			stepMotorDataLine5[7:0] = data[7:0];	
		end
		8'd13:
		begin
			stepMotorControlLine6[7:0] <= write_buf[7:0];
			stepMotorControlLine6[15:8] <=  data[7:0];	
		end
		8'd14:
		begin
			stepMotorDataLine6[7:0] = data[7:0];
		end
		8'd15:
		begin
			stepMotorControlLine7[7:0] <= write_buf[7:0];
			stepMotorControlLine7[15:8] <=  data[7:0];
		end
		8'd16:
		begin
			stepMotorDataLine7[7:0] <= data[7:0];
		end
		8'd17:
		begin
			stepMotorControlLine8[7:0] <= write_buf[7:0];
			stepMotorControlLine8[15:8] <=  data[7:0];
		end
		8'd18:
		begin
			stepMotorDataLine8[7:0] <= data[7:0];
		end
		8'd19:
		begin
			stepMotorControlLine9[7:0] <= write_buf[7:0];
			stepMotorControlLine9[15:8] <=  data[7:0];
		end
		8'd20:
		begin
			stepMotorDataLine9[7:0] <= data[7:0];
		end
		8'd21:
		begin
			stepMotorControlLine10[7:0] <= write_buf[7:0];
			stepMotorControlLine10[15:8] <=  data[7:0];
		end
		8'd22:
		begin
			stepMotorDataLine10[7:0] <= data[7:0];
		end
		8'd23:
		begin
			stepMotorControlLine11[7:0] <= write_buf[7:0];
			stepMotorControlLine11[15:8] <=  data[7:0];
		end
		8'd24:
		begin
			stepMotorDataLine11[7:0] <= data[7:0];
		end
		8'd25:
		begin
			stepMotorControlLine12[7:0] <= write_buf[7:0];
			stepMotorControlLine12[15:8] <=  data[7:0];
		end
		8'd26:
		begin
			stepMotorDataLine12[7:0] <= data[7:0];
		end
		8'd27:
		begin
			cdcA	<= data[7:0];
		end
		8'd28:	
		begin
			cdcB	<= data[7:0];
		end
		8'd29:	
		begin
			cdcC	<= data[7:0];
		end
		8'd30:	
		begin
			cdcD	<= data[7:0];
		end
		8'd31:	
		begin
			cdcE	<= data[3:0];
		end
		default:	;
		endcase
	end
end


always @(*)
begin
	case(addr)
	8'd0: dataOutReg = interrupt[7:0];
	8'd1: dataOutReg = {4'd0,interrupt[11:8]};
	8'd2: dataOutReg = nopA;
	8'd3: dataOutReg = {2'b00,nopB};
	8'd4: dataOutReg = 8'h55;//ID号1
	8'd5: dataOutReg = 8'hcc;//ID号2
	default:	dataOutReg = 8'hff;
	endcase
end


endmodule



//带减速的步进电机驱动模块
module StepMotorWithDAcc(
input rst_n,
input clk,
input [15:0]register,
input [7:0]pul_num,
output en,
output cw,
output pul,
output int_p
);
//output 
wire [12:0]count_set;
reg [15:0]counter;
reg out;
reg [8:0]pul_cnt;
reg pul_num_equal;
wire stop_n;
reg [15:0]register_act;
reg register_need_write;

assign count_set = register_act[12:0];
assign en = register_act[15:15];
assign cw  = register_act[14:14];
assign stop_n = register_act[13:13];


always@(posedge clk or negedge rst_n)
begin 
	if(!rst_n)
	begin
		counter <=  16'd0;
		pul_cnt <=  9'd1;
		pul_num_equal <=  1'd0;
		out <= 1;
	end
	else
	begin
		//实时更新寄存器数据,防止急停无效
		register_act <= register;
		if((stop_n)&&(pul_num_equal == 1'b0))
		begin
			//内部8分频
			if(counter == {count_set,3'b0})
			begin
				counter <= 16'd0;
				out <= ~out;
				pul_cnt <= pul_cnt + 1'b1;
				if(pul_cnt >= {pul_num[7:0],1'b0})//单脉冲式电机,必须两个时钟周期发一次脉冲
				begin
					pul_num_equal <=	1'b1;
				end
			end
			else
			begin
				counter <= counter+1'b1;
			end
		end
		else 
		begin
			if(stop_n == 1'b0)
			begin
				counter <= 16'd0;
				pul_cnt	<= 9'd1;
				pul_num_equal <= 1'd0;
			end
		end
	end
end

always@(posedge clk)
begin
	if(pul_num_equal == 1'd1)
	register_need_write <= 1'd1;
	else
	register_need_write <= 1'd0;
end

assign  pul = out;
assign int_p = register_need_write;
endmodule
