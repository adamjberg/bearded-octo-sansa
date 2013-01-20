LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
USE ieee.std_logic_unsigned.all;
ENTITY TowerDefence IS
PORT (
	CLOCK_50 : IN STD_LOGIC;
	KEY : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
	DRAM_CLK, DRAM_CKE : OUT STD_LOGIC;
	DRAM_ADDR : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
	DRAM_BA_0, DRAM_BA_1 : BUFFER STD_LOGIC;
	DRAM_CS_N, DRAM_CAS_N, DRAM_RAS_N, DRAM_WE_N : OUT STD_LOGIC;
	DRAM_DQ : INOUT STD_LOGIC_VECTOR(15 DOWNTO 0);
	DRAM_UDQM, DRAM_LDQM : BUFFER STD_LOGIC;
	SRAM_ADDR : OUT STD_LOGIC_VECTOR(17 downto 0);
	SRAM_DQ : INOUT STD_LOGIC_VECTOR(15 downto 0);
	SRAM_LB_N, SRAM_UB_N, SRAM_CE_N, SRAM_OE_N, SRAM_WE_N : OUT STD_LOGIC;
	VGA_R : OUT STD_LOGIC_VECTOR(9 downto 0);
	VGA_G : OUT STD_LOGIC_VECTOR(9 downto 0);
	VGA_B : OUT STD_LOGIC_VECTOR(9 downto 0);
	VGA_CLK, VGA_BLANK, VGA_HS, VGA_VS, VGA_SYNC : OUT STD_LOGIC;
	SD_CLK : OUT STD_LOGIC;
	SD_DAT3, SD_DAT, SD_CMD : INOUT STD_LOGIC
	);
END TowerDefence;

ARCHITECTURE struct OF TowerDefence IS
component nios_system is
port (
            clk_clk          : in    std_logic                     := 'X';             -- clk
            reset_reset_n    : in    std_logic                     := 'X';             -- reset_n
				sdram_clk_clk 	  : out   STD_LOGIC;
            sdram_wire_addr  : out   std_logic_vector(11 downto 0);                    -- addr
            sdram_wire_ba    : out   std_logic_vector(1 downto 0);                     -- ba
            sdram_wire_cas_n : out   std_logic;                                        -- cas_n
            sdram_wire_cke   : out   std_logic;                                        -- cke
            sdram_wire_cs_n  : out   std_logic;                                        -- cs_n
            sdram_wire_dq    : inout std_logic_vector(15 downto 0) := (others => 'X'); -- dq
            sdram_wire_dqm   : out   std_logic_vector(1 downto 0);                     -- dqm
            sdram_wire_ras_n : out   std_logic;                                        -- ras_n
            sdram_wire_we_n  : out   std_logic;                                         -- we_n
				sram_interface_DQ   : inout std_logic_vector(15 downto 0) := (others => 'X'); -- DQ
            sram_interface_ADDR : out   std_logic_vector(17 downto 0);                    -- ADDR
            sram_interface_LB_N : out   std_logic;                                        -- LB_N
            sram_interface_UB_N : out   std_logic;                                        -- UB_N
            sram_interface_CE_N : out   std_logic;                                        -- CE_N
            sram_interface_OE_N : out   std_logic;                                        -- OE_N
            sram_interface_WE_N : out   std_logic;                                        -- WE_N
            vga_extern_CLK      : out   std_logic;                                        -- CLK
            vga_extern_HS       : out   std_logic;                                        -- HS
            vga_extern_VS       : out   std_logic;                                        -- VS
            vga_extern_BLANK    : out   std_logic;                                        -- BLANK
            vga_extern_SYNC     : out   std_logic;                                        -- SYNC
            vga_extern_R        : out   std_logic_vector(9 downto 0);                     -- R
            vga_extern_G        : out   std_logic_vector(9 downto 0);                     -- G
            vga_extern_B        : out   std_logic_vector(9 downto 0);                      -- B
				sd_card_b_SD_cmd    : inout std_logic                     := 'X';             -- b_SD_cmd
            sd_card_b_SD_dat    : inout std_logic                     := 'X';             -- b_SD_dat
            sd_card_b_SD_dat3   : inout std_logic                     := 'X';             -- b_SD_dat3
            sd_card_o_SD_clock  : out   std_logic                   
);
end component nios_system;
SIGNAL DQM : STD_LOGIC_VECTOR(1 DOWNTO 0);
SIGNAL BA : STD_LOGIC_VECTOR(1 DOWNTO 0);
BEGIN
DRAM_BA_0 <= BA(0);
DRAM_BA_1 <= BA(1);
DRAM_UDQM <= DQM(1);
DRAM_LDQM <= DQM(0);
niosII : component nios_system
port map (
            clk_clk          => CLOCK_50,          --        clk.clk
            reset_reset_n    => KEY(0),    --      reset.reset_n
				sdram_clk_clk => DRAM_CLK,
            sdram_wire_addr  => DRAM_ADDR,  -- sdram_wire.addr
            sdram_wire_ba    => BA,    --           .ba
            sdram_wire_cas_n => DRAM_CAS_N, --           .cas_n
            sdram_wire_cke   => DRAM_CKE,   --           .cke
            sdram_wire_cs_n  => DRAM_CS_N,  --           .cs_n
            sdram_wire_dq    => DRAM_DQ,    --           .dq
            sdram_wire_dqm   => DQM,   --           .dqm
            sdram_wire_ras_n => DRAM_RAS_N, --           .ras_n
            sdram_wire_we_n  => DRAM_WE_N,   --           .we_n
				sram_interface_DQ   => SRAM_DQ,   -- sram_interface.DQ
            sram_interface_ADDR => SRAM_ADDR, --               .ADDR
            sram_interface_LB_N => SRAM_LB_N, --               .LB_N
            sram_interface_UB_N => SRAM_UB_N, --               .UB_N
            sram_interface_CE_N => SRAM_CE_N, --               .CE_N
            sram_interface_OE_N => SRAM_OE_N, --               .OE_N
            sram_interface_WE_N => SRAM_WE_N, --               .WE_N
            vga_extern_CLK      => VGA_CLK,      --     vga_extern.CLK
            vga_extern_HS       => VGA_HS,       --               .HS
            vga_extern_VS       => VGA_VS,       --               .VS
            vga_extern_BLANK    => VGA_BLANK,    --               .BLANK
            vga_extern_SYNC     => VGA_SYNC,     --               .SYNC
            vga_extern_R        => VGA_R,        --               .R
            vga_extern_G        => VGA_G,        --               .G
            vga_extern_B        => VGA_B,         --               .B
				sd_card_b_SD_cmd    => SD_CMD,    --        sd_card.b_SD_cmd
            sd_card_b_SD_dat    => SD_DAT,    --               .b_SD_dat
            sd_card_b_SD_dat3   => SD_DAT3,   --               .b_SD_dat3
            sd_card_o_SD_clock  => SD_CLK   --               .o_SD_clock
        );
end struct;
