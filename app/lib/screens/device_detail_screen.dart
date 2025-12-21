import 'package:flutter/material.dart';
import '../models/device.dart';
import '../services/device_service.dart';
import '../models/power_reading.dart';
import 'package:fl_chart/fl_chart.dart';

class DeviceDetailScreen extends StatefulWidget {
  final Device device;
  final DeviceService deviceService;

  const DeviceDetailScreen({
    super.key,
    required this.device,
    required this.deviceService,
  });

  @override
  State<DeviceDetailScreen> createState() => _DeviceDetailScreenState();
}

class _DeviceDetailScreenState extends State<DeviceDetailScreen> {
  List<PowerReading> _historicalData = [];
  bool _isLoading = true;

  @override
  void initState() {
    super.initState();
    _loadHistoricalData();
  }

  Future<void> _loadHistoricalData() async {
    final end = DateTime.now();
    final start = end.subtract(const Duration(hours: 24));
    
    final data = await widget.deviceService.getHistoricalData(
      widget.device.id,
      start: start,
      end: end,
    );

    setState(() {
      _historicalData = data;
      _isLoading = false;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(widget.device.name),
        actions: [
          IconButton(
            icon: const Icon(Icons.edit),
            onPressed: () {
              // TODO: Edit device name
            },
          ),
        ],
      ),
      body: SingleChildScrollView(
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.stretch,
          children: [
            // Device info card
            Card(
              margin: const EdgeInsets.all(16),
              child: Padding(
                padding: const EdgeInsets.all(16),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    const Text(
                      'Device Information',
                      style: TextStyle(
                        fontSize: 18,
                        fontWeight: FontWeight.bold,
                      ),
                    ),
                    const SizedBox(height: 16),
                    _buildInfoRow('Device ID', widget.device.id),
                    _buildInfoRow('Status', widget.device.status.toUpperCase()),
                    _buildInfoRow('Firmware', widget.device.firmwareVersion),
                    _buildInfoRow(
                      'Last Seen',
                      '${widget.device.lastSeen.hour}:${widget.device.lastSeen.minute.toString().padLeft(2, '0')}',
                    ),
                    const Divider(height: 24),
                    const Text(
                      'Security Status',
                      style: TextStyle(
                        fontSize: 16,
                        fontWeight: FontWeight.bold,
                      ),
                    ),
                    const SizedBox(height: 12),
                    _buildSecurityStatus(
                      'Encryption',
                      widget.device.isEncrypted,
                      'Data encrypted with AES-256-GCM',
                    ),
                    _buildSecurityStatus(
                      'Attestation',
                      widget.device.isAttested,
                      'Firmware integrity verified',
                    ),
                    _buildSecurityStatus(
                      'Tamper Detection',
                      !widget.device.tamperDetected,
                      widget.device.tamperDetected
                          ? 'Tamper alert triggered!'
                          : 'No tamper detected',
                    ),
                  ],
                ),
              ),
            ),

            // 24-hour power chart
            Card(
              margin: const EdgeInsets.symmetric(horizontal: 16),
              child: Padding(
                padding: const EdgeInsets.all(16),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    const Text(
                      '24-Hour Power Usage',
                      style: TextStyle(
                        fontSize: 18,
                        fontWeight: FontWeight.bold,
                      ),
                    ),
                    const SizedBox(height: 16),
                    if (_isLoading)
                      const Center(
                        child: Padding(
                          padding: EdgeInsets.all(32.0),
                          child: CircularProgressIndicator(),
                        ),
                      )
                    else
                      SizedBox(
                        height: 200,
                        child: LineChart(
                          LineChartData(
                            gridData: FlGridData(show: true),
                            titlesData: FlTitlesData(
                              leftTitles: AxisTitles(
                                sideTitles: SideTitles(showTitles: true),
                              ),
                              bottomTitles: AxisTitles(
                                sideTitles: SideTitles(showTitles: false),
                              ),
                              rightTitles: AxisTitles(
                                sideTitles: SideTitles(showTitles: false),
                              ),
                              topTitles: AxisTitles(
                                sideTitles: SideTitles(showTitles: false),
                              ),
                            ),
                            borderData: FlBorderData(show: true),
                            lineBarsData: [
                              LineChartBarData(
                                spots: _historicalData
                                    .asMap()
                                    .entries
                                    .map((e) => FlSpot(
                                          e.key.toDouble(),
                                          e.value.power,
                                        ))
                                    .toList(),
                                isCurved: true,
                                color: const Color(0xFF2563EB),
                                barWidth: 2,
                                dotData: FlDotData(show: false),
                              ),
                            ],
                          ),
                        ),
                      ),
                  ],
                ),
              ),
            ),

            const SizedBox(height: 16),

            // Control button
            Padding(
              padding: const EdgeInsets.all(16.0),
              child: ElevatedButton.icon(
                onPressed: () {
                  widget.deviceService.toggleDevice(widget.device.id);
                },
                icon: Icon(widget.device.isOn ? Icons.power_off : Icons.power),
                label: Text(widget.device.isOn ? 'Turn OFF' : 'Turn ON'),
                style: ElevatedButton.styleFrom(
                  padding: const EdgeInsets.all(16),
                  backgroundColor: widget.device.isOn
                      ? const Color(0xFFEF4444)
                      : const Color(0xFF10B981),
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }

  Widget _buildInfoRow(String label, String value) {
    return Padding(
      padding: const EdgeInsets.only(bottom: 8.0),
      child: Row(
        mainAxisAlignment: MainAxisAlignment.spaceBetween,
        children: [
          Text(
            label,
            style: TextStyle(
              fontSize: 14,
              color: Colors.grey[600],
            ),
          ),
          Text(
            value,
            style: const TextStyle(
              fontSize: 14,
              fontWeight: FontWeight.w500,
            ),
          ),
        ],
      ),
    );
  }

  Widget _buildSecurityStatus(String label, bool isSecure, String description) {
    return Padding(
      padding: const EdgeInsets.only(bottom: 12.0),
      child: Row(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          Icon(
            isSecure ? Icons.check_circle : Icons.warning,
            size: 20,
            color: isSecure ? const Color(0xFF10B981) : const Color(0xFFEF4444),
          ),
          const SizedBox(width: 12),
          Expanded(
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Text(
                  label,
                  style: const TextStyle(
                    fontSize: 14,
                    fontWeight: FontWeight.w500,
                  ),
                ),
                Text(
                  description,
                  style: TextStyle(
                    fontSize: 12,
                    color: Colors.grey[600],
                  ),
                ),
              ],
            ),
          ),
        ],
      ),
    );
  }
}
