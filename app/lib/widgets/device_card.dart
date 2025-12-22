import 'package:flutter/material.dart';
import '../models/device.dart';
import '../services/device_service.dart';
import '../theme/app_theme.dart';
import 'device_detail_screen.dart';

class DeviceCard extends StatelessWidget {
  final Device device;
  final DeviceService deviceService;

  const DeviceCard({
    super.key,
    required this.device,
    required this.deviceService,
  });

  @override
  Widget build(BuildContext context) {
    return Card(
      margin: const EdgeInsets.only(bottom: 16),
      child: InkWell(
        onTap: () {
          Navigator.of(context).push(
            MaterialPageRoute(
              builder: (_) => DeviceDetailScreen(
                device: device,
                deviceService: deviceService,
              ),
            ),
          );
        },
        borderRadius: BorderRadius.circular(12),
        child: Padding(
          padding: const EdgeInsets.all(16.0),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              // Header row
              Row(
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                children: [
                  Expanded(
                    child: Column(
                      crossAxisAlignment: CrossAxisAlignment.start,
                      children: [
                        Row(
                          children: [
                            Flexible(
                              child: Text(
                                device.name,
                                style: const TextStyle(
                                  fontSize: 18,
                                  fontWeight: FontWeight.bold,
                                ),
                                overflow: TextOverflow.ellipsis,
                              ),
                            ),
                            const SizedBox(width: 8),
                            // Encryption badge
                            if (device.isEncrypted)
                              Container(
                                padding: const EdgeInsets.symmetric(
                                  horizontal: 6,
                                  vertical: 2,
                                ),
                                decoration: BoxDecoration(
                                  color: AppTheme.encryptedBadgeColor.withOpacity(0.1),
                                  borderRadius: BorderRadius.circular(4),
                                ),
                                child: const Row(
                                  mainAxisSize: MainAxisSize.min,
                                  children: [
                                    Icon(Icons.lock, size: 10, color: AppTheme.encryptedBadgeColor),
                                    SizedBox(width: 2),
                                    Text(
                                      'Encrypted',
                                      style: TextStyle(
                                        fontSize: 10,
                                        color: AppTheme.encryptedBadgeColor,
                                        fontWeight: FontWeight.w500,
                                      ),
                                    ),
                                  ],
                                ),
                              ),
                          ],
                        ),
                        const SizedBox(height: 4),
                        Row(
                          children: [
                            Container(
                              width: 8,
                              height: 8,
                              decoration: BoxDecoration(
                                color: device.status == 'online'
                                    ? const Color(0xFF10B981)
                                    : Colors.grey,
                                shape: BoxShape.circle,
                              ),
                            ),
                            const SizedBox(width: 6),
                            Text(
                              device.status.toUpperCase(),
                              style: TextStyle(
                                fontSize: 12,
                                color: Colors.grey[600],
                                fontWeight: FontWeight.w500,
                              ),
                            ),
                          ],
                        ),
                      ],
                    ),
                  ),
                  // Power toggle
                  Switch(
                    value: device.isOn,
                    onChanged: device.status == 'online'
                        ? (value) {
                            deviceService.toggleDevice(device.id);
                          }
                        : null,
                    activeColor: const Color(0xFF10B981),
                  ),
                ],
              ),
              
              const Divider(height: 24),

              // Power metrics
              Row(
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                children: [
                  _buildMetric(
                    'Power',
                    '${device.currentPower.toStringAsFixed(1)} W',
                    Icons.bolt,
                    const Color(0xFFF59E0B),
                  ),
                  _buildMetric(
                    'Voltage',
                    '${device.voltage.toStringAsFixed(1)} V',
                    Icons.electric_bolt,
                    const Color(0xFF3B82F6),
                  ),
                  _buildMetric(
                    'Current',
                    '${device.current.toStringAsFixed(2)} A',
                    Icons.electric_meter,
                    const Color(0xFF8B5CF6),
                  ),
                ],
              ),

              // Security indicators
              if (device.tamperDetected || !device.isAttested) ...[
                const SizedBox(height: 12),
                Container(
                  padding: const EdgeInsets.all(8),
                  decoration: BoxDecoration(
                    color: const Color(0xFFEF4444).withOpacity(0.1),
                    borderRadius: BorderRadius.circular(6),
                  ),
                  child: Row(
                    children: [
                      const Icon(Icons.warning, size: 16, color: Color(0xFFEF4444)),
                      const SizedBox(width: 8),
                      Expanded(
                        child: Text(
                          device.tamperDetected
                              ? 'Tamper detected!'
                              : 'Device not attested',
                          style: const TextStyle(
                            fontSize: 12,
                            color: Color(0xFFEF4444),
                            fontWeight: FontWeight.w500,
                          ),
                        ),
                      ),
                    ],
                  ),
                ),
              ],
            ],
          ),
        ),
      ),
    );
  }

  Widget _buildMetric(String label, String value, IconData icon, Color color) {
    return Column(
      children: [
        Icon(icon, size: 20, color: color),
        const SizedBox(height: 4),
        Text(
          value,
          style: const TextStyle(
            fontSize: 16,
            fontWeight: FontWeight.bold,
          ),
        ),
        Text(
          label,
          style: TextStyle(
            fontSize: 11,
            color: Colors.grey[600],
          ),
        ),
      ],
    );
  }
}
